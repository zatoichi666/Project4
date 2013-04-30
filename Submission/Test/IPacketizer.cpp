#include <string>
#include <iostream>
#include "IPacketizer.h"

#ifdef TEST_IPACKETIZER

/////////////////////////////////////////////////////////////////////////
// Helper message converters

using namespace iPacketizer;

int main()
{
	String^ s = "RemoteSWRepo.vsd";
	PacketizerWrapper^ p = gcnew PacketizerWrapper(s);
	std::cout << ConvertMsgDown(p->getFileName());
	std::cout << ConvertMsgDown(p->operator[](5));

	return 0;	
}

#endif