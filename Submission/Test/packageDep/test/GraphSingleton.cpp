///////////////////////////////////////////////////////////////
// GraphSingleton.cpp - Graph Library                        //
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
//                                                           //
// Changes: Removed many inline functions, moved to          //
// implementation file for project 2                         //
//                                                           //
///////////////////////////////////////////////////////////////


#include "GraphXml.h"
#include "GraphSingleton.h"

#include <iostream>

using namespace GraphLib;

bool GraphSingleton::instanceFlag = false;
GraphSingleton* GraphSingleton::single = NULL;

typedef GraphLib::Graph<node, std::string> graph;
typedef GraphLib::Vertex<node, std::string> vertex;
typedef GraphLib::Display<node, std::string> display;
typedef GraphXml<node, std::string> graphXml;

void GraphSingleton::setCurrentFilename(std::string fn)
{
	currentFilename = fn;
}
std::string GraphSingleton::getCurrentFilename()
{
	return currentFilename;
}

graph GraphSingleton::getGraph()
{
	return *single;
}
GraphSingleton::~GraphSingleton()
{
	instanceFlag = false;
}

std::vector<size_t> GraphSingleton::graphSearchVertex(std::string value)
{
	std::vector<size_t> idVector;
	size_t idVert;
	std::cout << "Searching the graph for vertices containing: " << value << "\n";

	for(auto& vert : single->getGraph())
	{
		if (vert.value() == value)
		{
			idVert = vert.id();
			std::cout << "Found vertex with id: " << vert.id() << " and value: " << vert.value().payload << "\n";
			idVector.push_back(vert.id());
		}
	}
	return idVector;
}

void GraphSingleton::graphSearchEdge(std::string value)
{
	std::vector<size_t> idVector;
	graph g = single->getGraph();
	graph::iterator iter = g.begin();

	std::cout << "Searching the graph for edges containing: '" << value << "'\n";
	while(iter != g.end())
	{
		vertex v = *iter;
		std::string parent;
		parent = v.value().payload;
		for(size_t i=0; i<v.size(); ++i)
		{
			vertex::Edge edge = v[i];
			std::string child = g[edge.first].value().payload;
			std::string relationship = (edge.second);
			if (relationship == value)
			{
				std::cout << "Found edge with value: " << relationship << " to: " << g[edge.first].value().payload << " from: " << v.value().payload << "\n";
			}
		}
		++iter;
	}
}

bool GraphSingleton::doesEdgeExistByIds(size_t parentId, size_t childId)
{
	graph g = single->getGraph();
	graph::iterator iter = g.begin();
	bool parentFound;

	while(iter != g.end())
	{
		vertex v = *iter;
		std::string parent;
		parent = v.value().payload;

		parentFound = (v.id() == parentId);

		for (size_t i=0; i<v.size(); ++i)
		{
			vertex::Edge edge = v[i];
			std::string child = g[edge.first].value().payload;
			std::string relationship = (edge.second);
			if ((parentFound == true) && (g[edge.first].id() == childId )) 
				return true;
		}
		++iter;
	}
	return false;
}


size_t GraphSingleton::addTypeToGraph(std::string vertName)
{
	bool foundVert = false;
	size_t idVert;
	for(auto& vert : single->getGraph())
	{
		if (vert.value() == vertName)
		{
			foundVert = true;
			idVert = vert.id();
		}
	}
	if (foundVert == false)
	{
		vertex vv1(vertName,-1);
		single->addVertex(vv1);
		idVert = vv1.id();
	}
	return idVert;
}

void GraphSingleton::addRelationshipToGraph(std::string parentName, std::string childName, std::string relationship_s)
{
	if (parentName != childName)
	{
		GraphSingleton *s;
		s = GraphSingleton::getInstance();
		graph g = s->getGraph();

		bool foundParent = false;
		size_t idParent;
		bool foundChild = false;
		size_t idChild;

		for(auto& vert : s->getGraph())
		{
			if (vert.value() == parentName)
			{
				foundParent = true;
				idParent = vert.id();
			}
		}

		if (foundParent == false)
			idParent = s->addTypeToGraph(parentName);

		for(auto& vert : s->getGraph())
		{
			if (vert.value() == childName)
			{
				foundChild = true;
				idChild = vert.id();
			}
		}

		if (foundChild == false)
			idChild = s->addTypeToGraph(childName);

		bool edgeFound = false;
		for (size_t i=0; i<s->getGraph()[s->getGraph().findVertexIndexById(idParent)].size();i++)
			if (s->getGraph()[i].id() == idChild)
				edgeFound = true;

		if (!s->doesEdgeExistByIds(idParent, idChild))
		{
			s->addEdge(
				relationship_s, 
				s->getGraph()[s->getGraph().findVertexIndexById(idParent)], 
				s->getGraph()[s->getGraph().findVertexIndexById(idChild)]	);
		}
	}
}

std::string GraphSingleton::reducePathFileToFileNamePrefix(std::string pathFile)
{
	size_t endPos = pathFile.find_last_of(".");
	size_t startPos = pathFile.find_last_of("\\") + 1;
	return pathFile.substr(startPos, endPos - startPos);
}


#ifdef TEST_GRAPHSINGLETON

int main()
{
	std::cout << "Testing graphSingleton:\n";
	std::cout << "=======================\n";
	std::cout << "GraphSingleton shall(1) provide the ability to access     \n";
	std::cout << "a graph instance via the Singleton pattern.               \n\n";
	std::cout << " Creating two singleton graph instances: 1 and 2\n ================================================\n\n";
	GraphSingleton *sc1,*sc2;
	sc1 = GraphSingleton::getInstance();
	node test1_n("test1");
	node test2_n("test2");
	vertex vv1(test1_n,-1);
	vertex vv2(test2_n,-1);
	std::cout << " Adding a vertex to instance 1\n ================================================\n\n";
	sc1->addVertex(vv1);

	std::cout << " Adding a vertex to instance 2\n ================================================\n\n";
	sc2 = GraphSingleton::getInstance();
	sc2->addVertex(vv2);
	graph gg;

	gg = sc2->getGraph();
	std::cout << " Showing the complete graph via instance 2\n ================================================\n\n";
	display::show(gg);

	gg = sc1->getGraph();
	std::cout << "\n\n Showing the complete graph via instance 1\n ================================================\n\n";
	display::show(gg);

	getchar();
	return 0;
}

#endif