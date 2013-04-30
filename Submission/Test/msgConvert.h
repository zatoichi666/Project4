#ifndef MSGCONVERT_H
#define MSGCONVERT_H

#include <string>
#include <iostream>

using namespace System;
typedef String Message;

typedef std::string stdMessage;

stdMessage ConvertMsgDown(Message^ msg);
Message^ ConvertMsgUp(const stdMessage& smsg);

#endif