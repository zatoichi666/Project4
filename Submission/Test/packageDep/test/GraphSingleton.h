///////////////////////////////////////////////////////////////
// GraphSingleton.h - Graph Library                          //
// Ver 2.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Dell E6510, Windows 7                        //
// Application: CSE687 - OOD, Pr#2, Spring 2013              //
// Author:      Matt Synborski                               //
//              matthewsynborski@gmail.com                   //
///////////////////////////////////////////////////////////////
//                                                           //
// GraphSingleton shall(1) provide the ability to access     //
// a graph instance via the Singleton pattern.               //
//                                                           //
///////////////////////////////////////////////////////////////
// Changes: Removed many inline functions, moved to          //
// implementation file for project 2                         //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GRAPHSING_H
#define GRAPHSING_H

#include <iostream>
#include "Graph.h"
#include "GraphXml.h"

using namespace GraphLib;

typedef Graph<node, std::string> graph;
typedef Vertex<node, std::string> vertex;


class GraphSingleton : public graph  // implements GraphSingleton shall(1)
{
protected:
	static bool instanceFlag;
	static GraphSingleton* single;
	std::string currentFilename;

private:
	GraphSingleton()
	{
		//private constructor
		//instanceFlag = false;
		single = NULL;
	}
public:

	static GraphSingleton* GraphSingleton::getInstance()
	{
		if(! instanceFlag)
		{
			single = new GraphSingleton();
			instanceFlag = true;
			return single;
		}
		else
		{
			return single;
		}
	}

	void setCurrentFilename(std::string fn);
	std::string getCurrentFilename();
	//static GraphSingleton* getInstance();
	graph getGraph();
	~GraphSingleton();
	std::vector<size_t> graphSearchVertex(std::string value);
	void graphSearchEdge(std::string value);
	bool doesEdgeExistByIds(size_t parentId, size_t childId);
	size_t addTypeToGraph(std::string vertName);
	void addRelationshipToGraph(std::string parentName, std::string childName, std::string relationship_s);
	std::string reducePathFileToFileNamePrefix(std::string pathFile);
};



#endif