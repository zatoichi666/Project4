


#ifndef IPACKETIZER_H
#define IPACKETIZER_H

#include <string>
#include <iostream>
#include "Packetizer.h" // include "NativeClass.h"
#include "filefind.h"
#include "FileSystem.h"
#include "base64.h"
#include "md5.h"
#include "msgConvert.h"

namespace iPacketizer{

		public ref class PacketizerWrapper {
		Packetizer* p;

	public:
		PacketizerWrapper(String^ filename) { p = new Packetizer( ConvertMsgDown(filename)); }
		~PacketizerWrapper() { delete p; }
		String^ getFileName() { return ConvertMsgUp(p->getFileName()); }
		String^ operator[](size_t index) { return ConvertMsgUp(p->operator[](index)); }
		int size() { return p->size(); }
	};


}

#endif;