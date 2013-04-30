#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
///////////////////////////////////////////////////////////////
// ActionsAndRules.h                                         //
// Ver 2.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Dell E6510, Windows 7                        //
// Application: CSE687 - OOD, Pr#2, Spring 2013              //
// Author:      Matt Synborski                               //
//              matthewsynborski@gmail.com                   //
///////////////////////////////////////////////////////////////
//                                                           //
// Originally written by James Fawcett for CSE-687 adapted   // 
//                         by Matt Synborski                 //
//                                                           //
///////////////////////////////////////////////////////////////
/*
Module Operations: 
==================
This module defines several action classes.  Its classes provide 
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable. 

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();                 // and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 2.0 : 01 Jun 11
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06
- first release

*/
//

#include <queue>
#include <string>
#include <sstream>
#include <stack>
#include <iterator>

#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "GraphSingleton.h"
#include "SymbolTable.h"

using namespace GraphLib;

typedef GraphLib::Graph<node, std::string> graph;
typedef GraphLib::Vertex<node, std::string> vertex;

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
	std::string type;
	std::string name;
	size_t lineCount;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ")"; 
		return temp.str();
	}
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
	ScopeStack<element> stack;
	Toker* p_Toker;
	SymbolTable symb;
public:
	Repository(Toker* pToker)
	{
		p_Toker = pToker;
	}
	SymbolTable& symbolTable()
	{
		return symb;
	}	
	ScopeStack<element>& scopeStack()
	{
		return stack;
	}
	Toker* Toker()
	{
		return p_Toker;
	}
	size_t lineCount()
	{
		return (size_t)(p_Toker->lines());
	}
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--BeginningOfScope rule";
		if(pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{

		element elem;
		elem.type = "unknown";
		elem.name = "anonymous";
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if(pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if(p_Repos->scopeStack().size() == 0)
			return;
		element elem = p_Repos->scopeStack().pop();
	}
};


///////////////////////////////////////////////////////////////
// rule to detect #define " definitions

class DefineDefinition : public IRule
{
public:

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.length() > 2){
			if(tc[0] == "#" && tc[1] == "include" && tc[2] != "<")
			{

				doActions(pTc);
				return true;

			}

		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push union name onto ScopeStack

class PushDefine : public IAction
{
	Repository* p_Repos;

public:
	PushDefine(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{

		std::string name = (*pTc)[pTc->find("include") + 1];
		element elem;
		elem.type = "define";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);

		GraphSingleton *s;
		s = GraphSingleton::getInstance();
		std::cout << " \n#define detected, adding via pass 1: " << name << "\n";


		std::string child = name.substr(1,name.size()-1);
		child = child.substr(0,child.find("."));
		std::string parent = s->reducePathFileToFileNamePrefix(s->getCurrentFilename());
		
		
		s->addRelationshipToGraph(parent, child, "define");

	}
};


///////////////////////////////////////////////////////////////
//               End of Pass 2 Classes                       //
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		int len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for(int i=0; i<len+1; ++i)
			std::cout << (*pTc)[i] << " ";
	}
};

#endif
