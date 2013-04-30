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