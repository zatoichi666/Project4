/*

base64.cpp and base64.h

Copyright (C) 2004-2008 Ren� Nyffenegger

This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.

3. This notice may not be removed or altered from any source distribution.

Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

#include "base64.h"
#include <iostream>

const std::string base64::base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";


#ifdef TEST_BASE64

int main() {
	const std::string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschr�nkter Haftung" ;

	std::string encoded = base64::base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
	std::string decoded = base64::base64_decode(encoded);

	std::cout << "encoded: " << encoded << " string length " << encoded.size() << std::endl;
	std::cout << "decoded: " << decoded << " string length " << decoded.size() << std::endl;

	return 0;
}

#endif
