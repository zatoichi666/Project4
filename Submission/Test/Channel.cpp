/////////////////////////////////////////////////////////////////////////////
// Channel.cpp - Demonstrates socket communication per specification       //
//               for project 3, CSE-687, Spring 2013                       //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

#ifndef CHANNEL_H
#define CHANNEL_H

#include "Sender.h"
#include "Listener.h"

#include <iostream>
#include <thread>

class Channel
{
public:

	Channel::Channel(Receiver r, BinTalker s) : _recv(r), _sndr(s)	
	{ 
		
	}

	void Channel::sendFile(std::string ip, int port)
	{
		_sndr.start(ip, port);
	}

	void Channel::receiveFile(int listenPort)
	{
		_recv.start(listenPort);
	}

private:	
	Receiver _recv;
	BinTalker _sndr;	
};

#ifdef TEST_CHANNEL

int main()
{
	int listenPort = 8050;
	std::string listenIp = "127.0.0.1";
	int sendPort = 8080;
	std::string sendIp = "127.0.0.1";

	Receiver r;
	BinTalker t(Packetizer("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped2.scn"));
	Channel c(r, t);

	std::thread thr([&]() {c.receiveFile(listenPort);});	

	thr.join();

	sout << " done\n";
	return 0;
}

#endif
#endif