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
#include "Sender.h"

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
	SendThread(BQueue* pInQ, Socket* pSock) : pInQ_(pInQ), pSock_(pSock) {}
	void run()
	{
		while(true)
		{
			stdMessage msg = pInQ_->deQ();
			
			if(msg == "ChannelShutDown")
				break;

			pSock_->send(msg.c_str(),msg.size());
		}
	}
private:	
	BQueue* pInQ_;
	Socket* pSock_;
};


/////////////////////////////////////////////////////////////////////////
// Fawcett thread, used to make sure that will work with C++\CLI code

class Thread : public tthreadBase
{
public:
	Thread(BQueue* pInQ, BQueue* pOutQ) : pInQ_(pInQ), pOutQ_(pOutQ) {}
	void run()
	{
		while(true)
		{
			stdMessage msg = pInQ_->deQ();
			
			if(msg == "ChannelShutDown")
				break;


		}
	}
private:

	BQueue* pInQ_;
	BQueue* pOutQ_;
	

};

/////////////////////////////////////////////////////////////////////////
// ref class MockChannel echos messages from input queue to output queue

ref class MockChannel : public IChannel
{
public:
	MockChannel() : pInQ(new BQueue), pOutQ(new BQueue), sock_(new Socket)
	{
		//Thread* pThread = new Thread(pInQ, pOutQ);
		

		SendThread* pSendThread = new SendThread(pOutQ, sock_);
		sock_->connect("127.0.0.1",8050,false);

		pSendThread->start();
		
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
		stdMessage smsg = pOutQ->deQ();
		return ConvertMsgUp(smsg);
	}
	virtual void postMessage(Message^ msg) override
	{
		stdMessage smsg = ConvertMsgDown(msg);
		pInQ->enQ(smsg);
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
	BQueue* pInQ;
	BQueue* pOutQ;
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