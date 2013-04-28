///////////////////////////////////////////////////////////////////////
// MockChannel.cpp - echo messages from input queue to output queue  //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013         //
///////////////////////////////////////////////////////////////////////

#include "Sockets.h"
#include "MockChannel.h"
#include "BlockingQueue.h"
#include "Threads.h"
#include "locks.h"
//#include "Sender.h"

#include <string>
#include <iostream>


using namespace mockChannel;

/////////////////////////////////////////////////////////////////////////
// Helper message converters

typedef std::string stdMessage;

stdMessage ConvertMsgDown(Message^ msg)
{
	stdMessage smsg;
	for(int i=0; i<msg->Length; ++i)
		smsg += (int)msg[i];
	return smsg;
}

Message^ ConvertMsgUp(const stdMessage& smsg)
{
	Message^ msg;
	for(size_t i=0; i<smsg.size(); ++i)
		msg += (wchar_t)smsg[i];
	return msg;
}

typedef BlockingQueue<stdMessage> BQueue;

/////////////////////////////////////////////////////////////////////////
// Send thread

class SendThread : public tthreadBase
{
public:
	SendThread(BlockingQueue<std::string>* pInQ, Socket* pSock, std::string msg) : pInQ_(pInQ), pSock_(pSock),  msg_(msg)
	{ }
	void run()
	{

		TRACE("  SendThread sending: " + msg_ + " to " + pSock_->System().getRemoteIP(pSock_));

		pSock_->writeLine(msg_);

		TRACE("  Successfully wrote the message out");
	}
private:	
	std::string msg_;
	BlockingQueue<std::string>* pInQ_;
	Socket* pSock_;
};


class ClientHandlerThread : public tthreadBase
{

public:
	ClientHandlerThread(Socket s, BlockingQueue<std::string>& q) : s_(s), q_(q) {}
private:
	void run()
	{
		std::string msg;
		do
		{

			msg = s_.readLine();

			if (msg == "")
				break;
			q_.enQ(msg);  // This isn't happening

		} while (msg!= "quit");
	}
	Socket s_;
	BlockingQueue<std::string>& q_;
};

/////////////////////////////////////////////////////////////////////////
// Fawcett thread, used to make sure that will work with C++\CLI code

class ListenThread : public threadBase
{
public:
	ListenThread(int port, BlockingQueue<std::string>& pInQ) : q_(pInQ), listener_(port), stop_(false) {}
	void stop(bool end) { stop_ = end; }

private:

	void run()
	{
		while(!stop_)
		{

			SOCKET s = listener_.waitForConnect();
			TRACE("  Trying to create a ClientHandlerThread");			
			ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
			pCh->start();

		}

	}
	bool stop_;
	BlockingQueue<std::string>& q_;
	SocketListener listener_;
};

/////////////////////////////////////////////////////////////////////////
// ref class MockChannel echos messages from input queue to output queue

ref class MockChannel : public IChannel
{
public:
	MockChannel() : pInQ(new BlockingQueue<std::string>), pOutQ(new BlockingQueue<std::string>), sock_(new Socket)
	{
		ListenThread* pListenThread = new ListenThread(8080, *pOutQ);
		pListenThread->start();



	}
	~MockChannel()
	{
		delete pInQ;
		delete pOutQ;
		delete sock_;
	}

	virtual void shutDown() override
	{
		postMessage("ChannelShutDown");
	}

	virtual Message^ getMessage() override
	{
		stdMessage sMsg, qMsg;
		showQueues(qMsg);
		sMsg = pOutQ->deQ();
		return ConvertMsgUp(sMsg);
	}
	virtual void postMessage(Message^ msg) override
	{
		SendThread* pSendThread = new SendThread(pInQ, sock_, ConvertMsgDown(msg));

		if (!sock_->connect("127.0.0.1",8050,false))
		{
			TRACE("Couldn't complete connection");
			delete pSendThread;
		}
		else
			TRACE("Successfully completed connection");

		pSendThread->start();

	}

	void showQueues(const std::string& msg)
	{
		sout << locker << "\n  " << msg << unlocker;
		sout << locker << "\n inQ has " << pInQ->size() << " messages";
		sout << " and outQ has " << pOutQ->size() << " messages" << unlocker;
		sout.flush();
	}

private:
	Socket* sock_;
	BlockingQueue<std::string>* pInQ;
	BlockingQueue<std::string>* pOutQ;
};

IChannel^ IChannel::CreateChannel()
{
	return gcnew MockChannel();
}

int main()
{
	Message^ testMessage = "testMessage";
	stdMessage sTestMessage = ConvertMsgDown(testMessage);
	std::cout << "\n  " << sTestMessage;
	Message^ resultMessage = ConvertMsgUp(sTestMessage);
	Console::Write("\n  {0}\n", resultMessage);

	IChannel^ chan = IChannel::CreateChannel();
	Message^ msg = "a message";
	chan->postMessage(msg);
	Message^ retMsg = chan->getMessage();
	Console::Write("\n  {0}\n\n", retMsg);
	chan->shutDown();
}