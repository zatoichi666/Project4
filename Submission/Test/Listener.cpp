/////////////////////////////////////////////////////////////////////////////
// Listener.cpp - Demonstrates receive-side socket comms per specification //
//               for project 3, CSE-687, Spring 2013                       //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ostream>
#include <sstream>
#include <ctime>
#include <thread>

#include "Listener.h"
#include "FileSystem.h"
#include "base64.h"
#include "Sender.h"
#include "md5.h"

//For dependency scanner
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"
#include "filefind.h"
#include "FileSystem.h"
#include "GraphXml.h"

typedef GraphXml<node, std::string> graphXml;

//#define TRACING

void ClientHandlerThread::run()
{
	std::string msg;
	do
	{
		doLog("receive thread reading line");
		msg = s_.readLine();
		TRACE(msg);
		if(msg == "")
			break;
		q_.enQ(msg);
	} while(msg != "quit");	
}

void ListenThread::run()
{
	while(!stop_)
	{
		SOCKET s = listener_.waitForConnect();
		ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
		pCh->start();
	}
}

void Receiver::start(int port)
{

	sout << "Loading user accounts\n";

	std::string line;

	std::ifstream xmlFile ("users.xml");
	std::string infile;
	if (xmlFile.is_open())
	{
		while ( xmlFile.good() )
		{
			std::getline(xmlFile,line);
			infile += line;
		}
		xmlFile.close();
	}

	XmlReader rdr(infile);
	ul = AuthXml::readXml(rdr);

	sout << " Receiver: started on port " << port << "\n";
	pLt = new ListenThread(port, q_);
	try
	{
		pLt->start();
		std::string msg;
		int count = 0;
		do
		{
			msg = q_.deQ();
			processMessage(msg);
		} while(true);
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what();
		delete pLt;
	}
	catch(...)
	{
		sout << "\n  something bad happened";
		delete pLt;
	}
}

std::vector<FileSystem::File> Receiver::getFileList()
{
	return fileList;
}

//----< Act on the received message >--------------------------------------

void Receiver::processMessage(std::string message)
{
	size_t posOpenSquareBracket = message.find_first_of("[");
	size_t posMsgTypeSemicolon = message.find_first_of(";");

	std::string messageType = message.substr(posOpenSquareBracket+1,posMsgTypeSemicolon - posOpenSquareBracket - 1);

	if (messageType == "sendBin")
	{
		int isLastPacket = 0;
		FileSystem::FileInfo fi = processSendBinMsg(message, isLastPacket);
		if (isLastPacket)
			sout << " Receiver: Got the last packet of " << fi.name() << "\n";
	}
	if (messageType == "queryMd5")
		processQueryMd5Msg( message );
	
	if (messageType == "ackMd5")
		processAckMd5Msg( message );
	
	if (messageType == "ackBin")	
		processAckBinMsg( message );
	
	if (messageType == "loginReqMsg")
		processLoginRequestMessage( message );
	
	if (messageType == "newCheckin")
		processNewCheckin( message );	

	if (messageType == "checkinRequest")
		processCheckinRequestMsg( message );
	
	if (messageType == "queryPackages")
		processQueryPackageListMessage( message );

	if (messageType == "checkinCmd")
		processCheckinCmd( message );
}

void Receiver::processAckMd5Msg(std::string message )
{	
	/* ------------------------< Unpack the ackMd5 message >---------------------------------- */

	size_t posdIpHeader = message.find("ipSender='") + 10;
	size_t dIpEntryLength = message.find("'",posdIpHeader) - posdIpHeader;
	std::string dIp = message.substr(posdIpHeader,dIpEntryLength);

	size_t posdPortHeader = message.find("portSender='") + 12;
	size_t dPortEntryLength = message.find("'",posdPortHeader) - posdPortHeader;
	std::string dPort_s = message.substr(posdPortHeader,dPortEntryLength);

	int dPort;
	if ( ! (std::istringstream(dPort_s) >> dPort) ) dPort = 0;

	size_t posMd5Header = message.find("md5val='") + 8;
	size_t md5EntryLength = message.find("'",posMd5Header) - posMd5Header;
	std::string md5_s = message.substr(posMd5Header,md5EntryLength);

	sout << " ackMD5: " << md5_s << " from IP: " << dIp << " port: " << dPort << "\n";
}

void Receiver::processLoginRequestMessage(std::string message )
{
	size_t posuName = message.find("uName='") + 7;
	size_t uNameEntryLength = message.find("'",posuName) - posuName;
	std::string uName = message.substr(posuName,uNameEntryLength);

	size_t pospWord = message.find("pWord='") + 7;
	size_t pWordEntryLength = message.find("'",pospWord) - pospWord;
	std::string pWord = message.substr(pospWord,pWordEntryLength);

	bool authSuccess = false;
	user u = ul.getUserByUsername(uName);

	if (u.getAccesslevel() == 0 && u.getPassword() == pWord)
		authSuccess = true;

	if (authSuccess == true)
	{
		sout << "Authentication successful, " << uName << "\n";
		std::thread thr([&]() 
		{
			TextTalker ta;
			ta.start(ackLogin, "127.0.0.1", 8080, "1", "127.0.0.1", 8080 );
		});
		thr.join();
	}
	else
	{
		sout << "Authentication unsuccessful\n";
		std::thread thr([&]() 
		{
			TextTalker ta;
			ta.start(ackLogin, "127.0.0.1", 8080, "0", "127.0.0.1", 8080 );
		});
		thr.join();
	}
}

void Receiver::processQueryMd5Msg(std::string message )
{	
	size_t posFileHeader = message.find("file='") + 6;
	size_t fileEntryLength = message.find("'",posFileHeader) - posFileHeader;
	std::string fileName = message.substr(posFileHeader,fileEntryLength);
	size_t posdIpHeader = message.find("ipSender='") + 10;
	size_t dIpEntryLength = message.find("'",posdIpHeader) - posdIpHeader;
	std::string dIp = message.substr(posdIpHeader,dIpEntryLength);
	size_t posdPortHeader = message.find("portSender='") + 12;
	size_t dPortEntryLength = message.find("'",posdPortHeader) - posdPortHeader;
	std::string dPort_s = message.substr(posdPortHeader,dPortEntryLength);
	int dPort;
	if ( ! (std::istringstream(dPort_s) >> dPort) ) dPort = 0;
	sout << " queryMD5: " << fileName << " from IP: " << dIp << " port: " << dPort << "\n";

	FileSystem::FileInfo fi(fileName);	
	FileSystem::File inFile(fileName);
	inFile.open(FileSystem::File::in, FileSystem::File::binary);
	std::string fileContents;
	if(!inFile.isGood())
	{
		std::cout << "\n  can't open binary file\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << FileSystem::Path::getFullFileSpec(inFile.name()) << "\n";
	}
	else
		while(inFile.isGood())
		{
			FileSystem::Block b = inFile.getBlock(CHUNK_SIZE);
			std::string packet;
			for (size_t i=0;i<b.size();i++)
				packet.push_back(b[i]);
			fileContents += packet;
		}
		std::string fileMd5Value;
		std::thread thr([&]() 
		{
			std::clock_t start;
			double duration;
			start = std::clock();
			fileMd5Value = md5(fileContents.c_str());			
			duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
			sout << " queryMD5: Calculated md5: " << fileMd5Value << " in " << duration << " seconds.\n";
			TextTalker ta;
			ta.start(ackMd5, dIp, dPort, fileMd5Value, dIp, dPort );
		});
		thr.join();
}

void Receiver::processQueryPackageListMessage(std::string message)
{
	sout << "\n Got a queryPackageList message\n";
}
void Receiver::processCheckinCmd( std::string message )
{
	sout << "\n Got a checkinCmd\n";

}

void Receiver::processCheckinRequestMsg(std::string message)
{
	sout << "\n Got a checkin request.  Pending checkin has " << pC->size() << " files in it\n";
	scanPendingCheckinForDependencies();
	GraphSingleton *s;
	s = GraphSingleton::getInstance();
	graph g;
	g = s->getGraph();
	graphXml::printPrettyGraph(g);
	std::vector<std::string> missingPackages = getListOfMissingPackagesFromPendingCheckin();

	std::string payload = Messager::makeCheckinMissingPackageMessage(missingPackages,"127.0.0.1",8050);

	std::thread thr([&]() 
	{
		TextTalker ta;
		ta.start(missingPack, "127.0.0.1", 8080, payload, "127.0.0.1", 8050 );
	});
	thr.join();



}

/* ----------< Find packages in the graph that aren't in the pending checkin >---------------- */

std::vector<std::string> Receiver::getListOfMissingPackagesFromPendingCheckin()
{
	std::vector<std::string> presentFiles, difference_vec;
	GraphSingleton *s;
	s = GraphSingleton::getInstance();
	graph g = s->getGraph();
	graph::iterator iter = g.begin();
	while(iter != g.end())
	{
		vertex v = *iter;
		std::string parent;
		parent = v.value().payload;
		bool foundParent = false;
		for (size_t i=0;i<presentFiles.size();i++)
		{
			if (presentFiles[i] == parent)
				foundParent = true;
		}
		if (foundParent == false)
			presentFiles.push_back(parent);
		++iter;
	}
	iter = g.begin();
	std::vector<std::string> pendingCheckinCrunched;
	for (size_t i=0;i<pC->size();i++)
	{
		bool found = false;
		for (size_t j=0;j<pendingCheckinCrunched.size();j++)
		{
			if (pendingCheckinCrunched[j] == pC->operator[](i).substr(0,pC->operator[](i).find(".")))
				found = true;
		}
		if (found == false)
			pendingCheckinCrunched.push_back( pC->operator[](i).substr(0,pC->operator[](i).find(".")) );
	}

	for (size_t i=0;i<presentFiles.size();i++)
	{
		bool found = false;
		for (size_t j=0;j<pendingCheckinCrunched.size();j++)
		{
			if (presentFiles[i] == pendingCheckinCrunched[j])
				found = true;
		}
		if (found == false)
			difference_vec.push_back(presentFiles[i]);
	}	
	return difference_vec;
}

void Receiver::processNewCheckin(std::string message)
{

	size_t posFileHeader = message.find("checkinName='") + 13;
	size_t fileEntryLength = message.find("'",posFileHeader) - posFileHeader;
	std::string packName = message.substr(posFileHeader,fileEntryLength);
	sout << "\n Starting new checkin named " << packName << "\n";
	pC = new PendingCheckin;
}
void Receiver::processAckBinMsg(std::string message )
{	
	/* ------------------------< Unpack the ackBin message >---------------------------------- */

	size_t posFileHeader = message.find("file='") + 6;
	size_t fileEntryLength = message.find("'",posFileHeader) - posFileHeader;
	std::string fileName = message.substr(posFileHeader,fileEntryLength);

	size_t posdIpHeader = message.find("ipSender='") + 10;
	size_t dIpEntryLength = message.find("'",posdIpHeader) - posdIpHeader;
	std::string dIp = message.substr(posdIpHeader,dIpEntryLength);

	size_t posdPortHeader = message.find("portSender='") + 12;
	size_t dPortEntryLength = message.find("'",posdPortHeader) - posdPortHeader;
	std::string dPort_s = message.substr(posdPortHeader,dPortEntryLength);

	int dPort;
	if ( ! (std::istringstream(dPort_s) >> dPort) ) dPort = 0;

	sout << " ackBin: " << fileName << " from IP: " << dIp << " port: " << dPort << "\n";
}


//-------< Once the last SendBin message has been received, send an acknowledgement >-------------------

void Receiver::sendAckBinMsg(std::string fileName, int port, std::string ip )
{	
	std::thread thr([&]() 
	{
		TextTalker ta;
		ta.start(ackBin, ip, port, fileName, ip, port );
	});
	thr.join();
}

void Receiver::scanPendingCheckinForDependencies()
{
	ConfigParseToConsole configure;
	Parser* pParser;
	pParser = configure.Build();

	for(std::vector<std::string>::iterator iterTxt = pC->begin();
		iterTxt != pC->end();
		++iterTxt)
	{
		try
		{
			if(pParser)
			{
				if(!configure.Attach(*iterTxt))
				{
					std::cout << "\n  could not open file " << *iterTxt << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";

			}
			std::cout << "\n Pass 1: Parsing file: " << *iterTxt << "\n";

			GraphSingleton *s;
			s = GraphSingleton::getInstance();
			s->setCurrentFilename(*iterTxt);

			while(pParser->next())
				pParser->parse();
		}
		catch(std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
}

//----< process the SendBin Message >--------------------------------------

FileSystem::FileInfo Receiver::processSendBinMsg(std::string message, int& isLastPacket )
{
	size_t posFileHeader = message.find("file='") + 6;
	size_t fileEntryLength = message.find("'",posFileHeader) - posFileHeader;
	std::string fileName = message.substr(posFileHeader,fileEntryLength);
	size_t pospCountHeader = message.find("pCount='") + 8;
	size_t pCountEntryLength = message.find("'",pospCountHeader) - pospCountHeader;
	std::string pCount_s = message.substr(pospCountHeader,pCountEntryLength);
	int pCount;
	if ( ! (std::istringstream(pCount_s) >> pCount) ) pCount = 0;
	size_t pospIndHeader = message.find("pInd='") + 6;
	size_t pIndEntryLength = message.find("'",pospIndHeader) - pospIndHeader;
	std::string pInd_s = message.substr(pospIndHeader,pIndEntryLength);
	int pInd;
	if ( ! (std::istringstream(pInd_s) >> pInd) ) pInd = 0;
	size_t posdIpHeader = message.find("dIp='") + 5;
	size_t dIpEntryLength = message.find("'",posdIpHeader) - posdIpHeader;
	std::string dIp = message.substr(posdIpHeader,dIpEntryLength);
	size_t posdPortHeader = message.find("dPort='") + 7;
	size_t dPortEntryLength = message.find("'",posdPortHeader) - posdPortHeader;
	std::string dPort_s = message.substr(posdPortHeader,dPortEntryLength);
	int dPort;
	if ( ! (std::istringstream(dPort_s) >> dPort) ) dPort = 0;
	size_t posBinBegin = message.find("]") + 1;
	size_t posBinEnd = message.size();
	std::string binBlock = message.substr(posBinBegin,posBinEnd - posBinBegin);
	std::string binDec = base64::base64_decode(binBlock);
	FileSystem::Block b(&binDec[0],&binDec[binDec.size()]);
	FileSystem::File outFile(fileName);
	if (pInd == 0)
		outFile.open(FileSystem::File::out, FileSystem::File::binary);
	else
		outFile.open(FileSystem::File::append, FileSystem::File::binary);

	FileSystem::FileInfo fi(fileName);
	if(outFile.isGood())
		outFile.putBlock(b);
	if (pInd == pCount - 1)
	{
		isLastPacket = 1;
		pC->push_back(fileName);

		outFile.close();
		sendAckBinMsg(fileName, dPort, dIp);
	}
	else
	{
		isLastPacket = 0;
	}
	return fi;
}

#ifdef TEST_LISTENER

const std::string base64::base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

void main()
{
	int ret = 0;
	try
	{
		sout << "Waiting for Sender to send 2 .scn files (Requires simultaneous testing with Sender.exe)\n";
		Receiver rcvr;
		rcvr.start(8080);
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch(...)
	{
		sout << "\n  something bad happened";
		ret = 1;
	}
	sout << "\n\n";
}
#endif