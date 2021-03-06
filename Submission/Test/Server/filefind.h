/////////////////////////////////////////////////////////////////////////////
// filefind.h   - Recursively find files by extension                      //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

#ifndef FILEFIND_H
#define FILEFIND_H

#include <string>
#include <vector>
#include <iostream>

#include "FileSystem.h"

using namespace FileSystem;
std::vector<std::string> findFilesWithExtensionsMatching(std::vector<std::string> fileExtensio