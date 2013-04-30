///////////////////////////////////////////////////////////////
// AuthenticateXml.h - Authentication Library                //
// Ver 1.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Dell E6510, Windows 7                        //
// Application: CSE687 - OOD, Pr#4, Spring 2013              //
// Author:      Matt Synborski                               //
//              matthewsynborski@gmail.com                   //
///////////////////////////////////////////////////////////////
//                                                           //
//                                                           //
///////////////////////////////////////////////////////////////
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef AUTHXML_H
#define AUTHXML_H

#include <vector>
#include <list>
#include <stack>
#include <sstream>
#include <algorithm>

#include <iomanip>
#include "XmlReader.h"
#include "XmlWriter.h"
#include <unordered_map>



struct user
{

	std::string username;
	std::string password;
	int accesslevel;

	user() {}

	user(std::string username_, std::string password_, int accesslevel_) : username(username_), password(password_), accesslevel(accesslevel_) {}

	std::string getUsername()
	{
		return username;
	}

	std::string getPassword()
	{
		return password;
	}

	int getAccesslevel()
	{
		return accesslevel;
	}

};

typedef struct std::vector<user> uList_s;

class userList
{
	uList_s ul;

public:
	void addUser(user u)
	{
		ul.push_back(u);
	}

	user getUserByUsername(std::string username)
	{ 
		for (size_t i=0;i<ul.size();i++)
		{
			if (ul[i].username == username)
				return ul[i];
		}
		return user("error", "error", -1);
	}

	uList_s getUserList()
	{
		return ul;
	}

	void prettyPrintUserList()
	{
		std::cout << std::setw(20) << "User";
		std::cout << std::setw(20) << "Password";
		std::cout << std::setw(20) << "Access";
		std::cout << std::endl;
		std::cout << std::string(80, '_') << std::endl;


		for (size_t i=0;i<ul.size();i++)
		{
			std::cout << std::setw(20) << ul[i].getUsername();
			std::cout << std::setw(20) << ul[i].getPassword();
			std::cout << std::setw(20) << ul[i].getAccesslevel();
			std::cout << std::endl;
		}
	}
};

class AuthXml  
{
public:



	static userList readXml(XmlReader rdr) // implements GraphXml shall(2)
	{
		userList ul;
		rdr.reset();
		std::string str;	

		while (rdr.next())
		{
			user u;
			std::stringstream accesslev_ss;
			str = rdr.tag();
			if (str == "userAuth")
			{

				XmlReader::attribElems attrib = rdr.attributes();
				u.username = attrib[2].second;
				u.password = attrib[1].second;
				accesslev_ss << attrib[0].second;
				accesslev_ss >> u.accesslevel;
				ul.addUser(u);
			}
		}	
		return ul;
	}



	static void writeXml(std::string filename, userList ul) 
	{
		XmlWriter wtr;
		wtr.addDeclaration();
		wtr.addComment("Matthew Synborski");
		wtr.start("UserAuthenticationFile");



		//uList_s::iterator iter = ul.getUserList().begin();
		//while(iter != ul.getUserList().end())

		for (size_t i=0;i<ul.getUserList().size();i++)
		{
			XmlWriter vertXml;
			vertXml.start("userAuth");

			user u = ul.getUserList()[i];
			std::string t1;

			std::stringstream accesslev_ss;
			accesslev_ss << u.accesslevel;
			accesslev_ss >> t1;
			vertXml.addAttribute("accesslevel",t1);

			t1 = u.password;
			vertXml.addAttribute("password",t1);

			t1 = u.username;
			vertXml.addAttribute("username",t1 );

			vertXml.end();
			wtr.addBody(vertXml.xml());

		}
		wtr.end();
		std::ofstream out(filename);
		if(out.good())
		{
			out << wtr.xml().c_str();
			out.close();
		}
	}


};

#endif