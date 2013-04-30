///////////////////////////////////////////////////////////////
// GraphXml.cpp - Graph Library                                //
// Ver 2.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Dell E6510, Windows 7                        //
// Application: CSE687 - OOD, Pr#2, Spring 2013              //
// Author:      Matt Synborski                               //
//              matthewsynborski@gmail.com                   //
///////////////////////////////////////////////////////////////
//                                                           //
// GraphXml shall(1) provide the ability to write            //
// graph instances to .Xml files within the file system.     //
//                                                           //
// GraphXml shall(2) provide the ability to read             //
// graph instances to .Xml files within the file system.     //
//                                                           //
// GraphXml shall(3) provide the Tarjan algorithm            //
// on graph instances.                                       //
//                                                           //
// GraphXml shall(4) perform a topological sort on strongly  //
// connected components.                                     //
//                                                           //
// GraphXml shall(5) condense the sorted (4) SCCs into a     //
// new directed graph                                        //
//                                                           //
///////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

#include "GraphXml.h"
#include "XmlReader.h"
#include "XmlWriter.h"

using namespace GraphLib;

typedef Graph<node, std::string> graph;
typedef Vertex<node, std::string> vertex;
typedef Display<node, std::string> display;
typedef GraphXml<node, std::string> graphXml;
typedef TarjanAlgorithm<node, std::string> tarjanAlgorithm;
typedef TopoSort<node, std::string> topoSort;


template<typename V, typename E>
void showVert(Vertex<V,E>& v)
{
	std::cout << "\n  " << v.value().payload;
}





std::string node::c_str()
{
	std::stringstream ss_lowIndex, ss_index;
	std::string lowIndex_s, index_s;
	ss_lowIndex << lowIndex;
	ss_lowIndex >> lowIndex_s;
	ss_index << index;
	ss_index >> index_s;
	return "Value : " + payload;		
}
//--------< constructor >----------------------------------------------------------------------
node::node(const std::string str)
{
	payload = str;
	lowIndex = SIZE_MAX;
	index = SIZE_MAX;
	mark = unvisited;
}

//--------< destructor >-----------------------------------------------------------------------
//node::~node(void)  allow default compiled destructor


//--------< equal comparator >-----------------------------------------------------------------
bool node::operator== (const node &n1)
{
	return ((n1.payload == payload) &&
		(n1.lowIndex == lowIndex) &&
		(n1.index == index));
}

//--------< not equal comparator >-------------------------------------------------------------
bool node::operator!= (const node &n1)
{
	return !(operator== (n1));
}





void tarjanAlgorithm::strongConnect(vertex v)
{
	size_t nodeVindex = dg.findVertexIndexById(v.id());
	dg[nodeVindex].value().index = index;
	dg[nodeVindex].value().lowIndex = index;
	index++;
	Stk.push_back(dg[nodeVindex]);	
	for(size_t i=0; i<dg[nodeVindex].size(); ++i)
	{
		vertex::Edge edge = dg[nodeVindex][i];
		size_t nodeWindex = edge.first;
		bool ContainsW = false; //search the stack for dg[edge.first]
		for (size_t j=0; j<Stk.size(); j++)
		{
			if (Stk[j].value() == dg[nodeWindex].value())
				ContainsW = true;
		}
		if ( dg[nodeWindex].value().index == SIZE_MAX )
		{
			strongConnect( dg[nodeWindex] );	
			if (dg[nodeVindex].value().lowIndex != SIZE_MAX)	{
				dg[nodeVindex].value().lowIndex = 
					std::min(dg[nodeVindex].value().lowIndex,dg[nodeWindex].value().lowIndex);
			}
		}
		else if (ContainsW)
		{ 				
			if (dg[nodeVindex].value().lowIndex != SIZE_MAX){
				dg[nodeVindex].value().lowIndex = 
					std::min( dg[nodeVindex].value().lowIndex,dg[nodeWindex].value().index); 
			}
		}
	}
	if (dg[nodeVindex].value().lowIndex == dg[nodeVindex].value().index)
	{
		std::vector<vertex> scc;
		node n("z");
		vertex w(n);			
		do
		{		
			w = Stk.back();
			scc.push_back(w);
			Stk.pop_back();
		} while (dg[nodeVindex].value() != w.value());
		stronglyConnectedComponents.push_back(scc);
	}
}

std::vector<std::vector<vertex>> tarjanAlgorithm::getSCC()
{
	return stronglyConnectedComponents;
}

std::vector<std::vector<vertex>> tarjanAlgorithm::tarjan(graph g)
{
	stronglyConnectedComponents.clear();
	index = 0;
	Stk.clear();
	dg = g;
	for (auto vert: dg)
	{
		if (vert.value().index == SIZE_MAX)
			strongConnect(vert);			
	}
	return stronglyConnectedComponents;
}

	void topoSort::visit(vertex v)
	{		
		if (v.value().mark == temporary)
		{
			std::cout << "\n\n (!!!) TopoSort: Stopping, not a DAG (!!!) \n";
		}
		else if (v.value().mark == unvisited)
		{
			dg[dg.findVertexIndexById(v.id())].value().mark = temporary;
			for (size_t i=0;i<v.size();i++)
			{
				vertex::Edge edge = v[i];
				vertex child = dg[edge.first];
				visit(child);
			}
			dg[dg.findVertexIndexById(v.id())].value().mark = permanent;
			topoSortList.push_back(v);
		}

	}

	vertex topoSort::findUnmarkedNode(int& ret)
	{
		ret = 0;
		for (auto vert: dg)
		{
			if (vert.value().mark != permanent)
			{
				ret++;
				return dg[dg.findVertexIndexById(vert.id())];
			}
		}
		//all vertices are marked permanent
		node terminatorNode("terminator");
		vertex terminator(terminatorNode);
		return terminator;
	}

	std::vector<vertex> topoSort::getTopoSortList()
	{
		return topoSortList;
	}
	void topoSort::topoSort(graph g)
	{
		dg = g;
		int ret = 0;
		vertex v = findUnmarkedNode(ret);
		while (ret > 0)
		{
			v = findUnmarkedNode(ret);
			if (v.value().payload != "terminator")
				visit(v);
		}
	}

#ifdef TEST_GRAPHXML

int main()
{
	std::cout << "\nTesting GraphXml Library";
	std::cout << "\n=======================\n\n";
	std::cout << "Testing GraphXml shall(1) provide the ability to write \n";
	std::cout << "graph instances to .Xml files within the file system.  \n\n";        
	std::cout << " Creating a new graph";
	std::cout << "\n Testing XmlReader";
	std::cout << "\n ===================\n";
	graph fGraph;


	//http://upload.wikimedia.org/wikipedia/commons/thumb/5/5c/Scc.png/220px-Scc.png
	node n("a");
	vertex a(n);
	n.payload = "b";
	vertex b(n);
	n.payload = "c";
	vertex c(n);
	n.payload = "d";
	vertex d(n);
	n.payload = "e";
	vertex e(n);
	n.payload = "f";
	vertex f(n);
	n.payload = "g";
	vertex g(n);
	n.payload = "h";
	vertex h(n);

	fGraph.addVertex(a);
	fGraph.addVertex(b);
	fGraph.addVertex(c);
	fGraph.addVertex(d);
	fGraph.addVertex(e);
	fGraph.addVertex(f);
	fGraph.addVertex(g);
	fGraph.addVertex(h);


	fGraph.addEdge("e1",a,b);
	fGraph.addEdge("e2",b,c);
	fGraph.addEdge("e10",c,d);
	fGraph.addEdge("e12",d,h);
	fGraph.addEdge("e13",h,g);
	fGraph.addEdge("e8",g,f);
	fGraph.addEdge("e3",e,a);
	fGraph.addEdge("e4",b,f);
	fGraph.addEdge("e5",e,f);
	fGraph.addEdge("e6",b,e);
	fGraph.addEdge("e7",c,g);
	fGraph.addEdge("e9",f,g);
	fGraph.addEdge("e11",d,c);
	fGraph.addEdge("e14",h,d);

	display::show(fGraph);

	graphXml::writeXml("test.xml",fGraph);

	std::cout << "\n\n Testing GraphXml shall(2) provide the ability to read \n";
	std::cout << " graph instances from .Xml files within the file system. ";       
	std::cout << "\n\n Reading the .xml file from the file system.\n";
	std::cout << "----------------------------------\n";
	std::string line;
	std::ifstream xmlFile ("test.xml");
	std::string infile;
	if (xmlFile.is_open())
	{
		while ( xmlFile.good() )
		{
			std::getline(xmlFile,line);
			infile += line;
		}
		xmlFile.close();
	}
	std::cout << infile << std::endl;

	graph gGraph;
	XmlReader rdr(infile);
	gGraph = graphXml::readXml(rdr);
	display::show(gGraph);
	// GraphXml shall(3) provide the Tarjan algorithm            //
	// on graph instances.                                       //
	std::cout << "\n\n Testing GraphXml shall(3) provide the Tarjan algorithm \n";
	std::cout << " on graph instances.  ";       
	std::cout << "\n\n Expected result: 3 strongly connected components.\n";
	std::cout << "---------------------------------------------------\n";

	tarjanAlgorithm tarjObj;

	std::cout << " Actual result: " << tarjObj.tarjan(gGraph).size() << " strongly connected components.\n";	

	std::cout << " GraphXml shall(5) condense the sorted (4) SCCs into a new directed graph \n";

	std::cout << "\n Expected result: non-cyclic graph\n";
	std::cout << " 1 -> 0\n";
	std::cout << " 2 -> 1\n";
	std::cout << " 2 -> 0\n";

	std::cout << " Demonstrating condensedGraph()\n";
	std::cout << "--------------------------------\n";
	graph g3 = graphXml::condensedGraph(tarjObj.getSCC(),gGraph);

	display::show(g3);

	std::cout << "\n\n Demonstrating collapseSccIntoString()\n";
	std::cout << "---------------------------------------\n";
	std::cout << "  SCC 0: " << graphXml::collapseSccIntoString( tarjObj.getSCC()[0] ) << "\n";
	std::cout << "  SCC 1: " << graphXml::collapseSccIntoString( tarjObj.getSCC()[1] ) << "\n";
	std::cout << "  SCC 2: " << graphXml::collapseSccIntoString( tarjObj.getSCC()[2] ) << "\n";

	std::cout << "\n\n Testing GraphXml shall(4) perform a topological sort on strongly connected components. \n";


	topoSort topoObj;
	topoObj.topoSort(g3);


	std::cout << "\n\n Demonstrating TopoSort on the condensed graph resulting from the above.\n";
	std::cout << "----------------------------\n";
	std::cout << "  Sorted Graph: Vertex 0: " << topoObj.getTopoSortList()[0].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 1: " << topoObj.getTopoSortList()[1].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 2: " << topoObj.getTopoSortList()[2].value().payload  << "\n";

	topoSort topoObj2;
	graph topoGraph2;

	node n2("a");
	vertex a2(n2);
	n2.payload = "b";
	vertex b2(n2);
	n2.payload = "c";
	vertex c2(n2);
	n2.payload = "d";
	vertex d2(n2);
	n2.payload = "e";
	vertex e2(n2);
	n2.payload = "f";
	vertex f2(n2);
	n2.payload = "g";
	vertex g2(n2);
	n2.payload = "h";
	vertex h2(n2);

	topoGraph2.addVertex(a2);
	topoGraph2.addVertex(b2);
	topoGraph2.addVertex(c2);
	topoGraph2.addVertex(d2);
	topoGraph2.addVertex(e2);
	topoGraph2.addVertex(f2);
	topoGraph2.addVertex(g2);
	topoGraph2.addVertex(h2);

	topoGraph2.addEdge("0", a2, b2);
	topoGraph2.addEdge("1", a2, c2);
	topoGraph2.addEdge("2", b2, d2);
	topoGraph2.addEdge("3", b2, f2);
	topoGraph2.addEdge("4", c2, f2);
	topoGraph2.addEdge("5", c2, e2);
	topoGraph2.addEdge("6", g2, e2);
	topoGraph2.addEdge("7", g2, h2);

	std::cout << "Showing the 8-node a-cyclical graph\n";
	std::cout << "-----------------------------------\n";
	display::show(topoGraph2);
	std::cout << "\n";

	topoObj2.topoSort(topoGraph2);
	std::cout << "\n\n Demonstrating TopoSort on an 8-node a-cyclical graph.\n";
	std::cout << "----------------------------\n";
	std::cout << "  Sorted Graph: Vertex 0: " << topoObj2.getTopoSortList()[0].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 1: " << topoObj2.getTopoSortList()[1].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 2: " << topoObj2.getTopoSortList()[2].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 3: " << topoObj2.getTopoSortList()[3].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 4: " << topoObj2.getTopoSortList()[4].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 5: " << topoObj2.getTopoSortList()[5].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 6: " << topoObj2.getTopoSortList()[6].value().payload  << "\n";
	std::cout << "  Sorted Graph: Vertex 7: " << topoObj2.getTopoSortList()[7].value().payload  << "\n";


	return 0;
}

#endif