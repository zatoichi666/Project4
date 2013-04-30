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
	std::cout << p->getFileName();

	return 0;	
}

#endif