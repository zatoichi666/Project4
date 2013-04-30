#include "msgConvert.h"


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