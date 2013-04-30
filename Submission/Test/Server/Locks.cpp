///////////////////////////////////////////////////////////////
// Locks.h - Define Lock classes based on:                   //
//             CriticalSection, SlimReaderWriter, Mutex      // 
// ver 1.1                                                   //
// Language: standard C++                                    //
// Platform: Dell Dimension T7400, Windows 7, SP #1          //
// Application: Resource for DO projects                     //
// Author:      Jim Fawcett, CST 4-187, Syracuse University  //
//              jfawcett@twcny.rr.com, (315) 443-3948        //
///////////////////////////////////////////////////////////////

#include "Locks.h"
#include <iostream>

//----< globally available output stream >---------------------------

syncOut sout(std::cout);  // globally available sout object
                          // mimics cout

#ifdef TEST_LOCKS

#include "Threads.h"
#include "HiResTimer.h"

///////////////////////////////////////////////////////////////
// test derived