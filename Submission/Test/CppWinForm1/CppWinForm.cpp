/////////////////////////////////////////////////////////////////////////
// CppWinForm.cpp - Demo of techniques to build C++ WinForm interface  //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2007           //
/////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Form1.h"

using namespace mockChannel;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
  // Enabling Windows XP visual effects before any controls are created
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false); 

  // Create the main window and run it
  Application::Run(gcnew Form1());
  return 0;
}
