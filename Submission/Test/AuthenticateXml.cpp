///////////////////////////////////////////////////////////////
// AuthenticateXml.cpp - Authentication Library              //
// Ver 2.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Dell E6510, Windows 7                        //
// Application: CSE687 - OOD, Pr#4, Spring 2013              //
// Author:      Matt Synborski                               //
//              matthewsynborski@gmail.com                   //
///////////////////////////////////////////////////////////////
//                                                           //
//                                                           //
///////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

#include "AuthenticateXml.h"
#include "XmlReader.h"
#include "XmlWriter.h"







#ifdef TEST_AUTHXML

int main()
{
	std::cout << "\nTesting AuthXml Library";
	std::cout << "\n=======================\n\n";
	std::cout << " Creating a couple new users\n\n";

	userList ulst;


	user u1("user1","pass1",1);
	user u2("user2","pass2",2);
	user u3("admin","password",0);

	ulst.addUser(u1);
	ulst.addUser(u2);
	ulst.addUser(u3);

	std::cout << "Finding user1's access level, should be 1: " << ulst.getUserByUsername("user1").getAccesslevel() << "\n";
	std::cout << "Finding user2's access level, should be 2: " << ulst.getUserByUsername("user2").getAccesslevel() << "\n";

	AuthXml::writeXml("test.xml",ulst);



	std::cout << "----------------------------------\n";
	std::string line;
	std::ifstream xmlFile ("test.xml");
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
	std::cout << infile << std::endl;

	XmlReader rdr(infile);
	userList ulst2 = AuthXml::readXml(rdr);
	std::cout << "\nPrinting user list\n";
	std::cout << "----------------------------------\n";

	ulst2.prettyPrintUserList();

	return 0;
}

#endif