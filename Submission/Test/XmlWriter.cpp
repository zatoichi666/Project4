///////////////////////////////////////////////////////////////////////
// XmlWriter.cpp - Create XML Strings                                //
// ver 4                                                             //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: Summer 2011 Projects                                 //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              www.lcs.syr.edu/faculty/fawcett, (315) 443-3948      //
///////////////////////////////////////////////////////////////////////

#include "XmlWriter.h"

//----< add opening tag >--------------------------------------------

XmlWriter& XmlWriter::start(const std::string& tag)
{
  _tag = tag;
  _xml += '<' + tag + '>';
  return *this;
}
//----< add attribute to opening tag >-------------------------------
//
//  Must be applied before adding body or closing
//
XmlWriter& XmlWriter::addAttribute(const std::string& name, const std::string& value)
{
  position = _xml.find_last_of('>');
  if(_xml[--position] == '/')
  {
    _xml.insert(position,'"' + value + '"' + ' ');
    _xml.insert(position,name + '=');
  }
  else
  {
    _xml.insert(++position,'"' + value + '"');
    _xml.insert(position,' ' + name + '=');
  }
  return *this;
}
//----< add body string to opening tag >-----------------------------

XmlWriter& XmlWriter::addBody(const std::string& body)
{
  _xml += body;
  return *this;
}
//----< add closing tag >--------------------------------------------

XmlWriter& XmlWriter::end()
{
  _xml += "</" + _tag + '>';
  return *this;
}
//----< construct element from tag and body >------------------------

XmlWriter& XmlWriter::element(const std::string& tag, const std::string& body)
{
  start(tag);
  addBody(body);
  end();
  return *this;
}
//----< add XML declaration >----------------------------------------
//
//  May be invoked once anytime during construction
//
XmlWriter& XmlWriter::addDeclaration()
{
  _xml = "<?xml version=\"1.0\"?>" + _xml;
  return *this;
}
//----< add XML comment ---------------------------------------------
//
//  Will be placed at the end of XML constructed so far
//
XmlWriter& XmlWriter::addComment(const std::string& cmt)
{
  _xml += "<!-- " + cmt + " -->";
  return *this;
}

#ifdef TEST_XMLWRITER

#include <iostream>
#include <fstream>

void mOut(const std::string& message)
{
  std::cout << "\n  " << message.c_str();
}

void main()
{
  mOut("Testing XmlWriter");
  mOut("=================");

  mOut("creating XML writer object:"); 
  XmlWriter wtr;

  mOut("adding XML declaration:");
  wtr.addDeclaration();
  mOut(wtr.xml());

  mOut("adding comment:");
  wtr.addComment("top level comment");
  mOut(wtr.xml());

  mOut("adding root element:");
  wtr.start("root");
  mOut(wtr.xml());

  mOut("adding attributes:");
  wtr.addAttribute("att1","val1");
  wtr.addAttribute("att2","val2");
  mOut(wtr.xml());

  mOut("adding comment:");
  wtr.addComment("comment in root's body");
  mOut(wtr.xml());

  mOut("Creating self-closing element:");
  XmlWriter sub1;
  sub1.start("child1 /");
  mOut(sub1.xml());

  mOut("adding attribute:");
  sub1.addAttribute("c1name","c1value");
  mOut(sub1.xml());
  
  mOut("adding child to root's body:");
  wtr.addBody(sub1.xml());
  mOut(wtr.xml());

  mOut("adding another comment");
  wtr.addComment("another root's body comment");
  mOut(wtr.xml());

  mOut("adding string to root's body:"); 
  wtr.addBody("root's body");
  mOut(wtr.xml());

  mOut("closing root element:\n");
  wtr.end();
  mOut(wtr.xml());

  mOut("\n  writing XML to file \"Test.xml\":");
  std::ofstream out("test.xml");
  if(out.good())
  {
    out << wtr.xml().c_str();
    out.close();
  }
  std::cout << std::endl;

  mOut("creating composite element:");
  XmlWriter cwtr, bcwtr1, bcwtr2;
  std::string temp = 
    bcwtr1.element("child1","child1's body")
    .element("child2","child2's body").xml();
  std::cout << "\n  " << bcwtr1.xml();
  bcwtr1.clear();

  std::cout << "\n  " <<
    cwtr.start("parent")
    .addBody(bcwtr1.element("child1","child1's body").xml())
    .addBody(bcwtr2.element("child2","body2").xml())
    .end().xml();
  std::cout << "\n\n";
}
#endif
