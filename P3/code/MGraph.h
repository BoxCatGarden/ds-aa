// MGraph.h: interface for the MGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGRAPH_H__CA1C7F68_2639_4E1D_9522_08519854E06E__INCLUDED_)
#define AFX_MGRAPH_H__CA1C7F68_2639_4E1D_9522_08519854E06E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

class MGraph : public Graph  
{
	const int nV;
	int nE;
	double** adjMatrix;
	char* mark;
public:
	MGraph(int vertexNum);
	virtual ~MGraph();

	//return the number of vertexes in the graph
	int n() const {return nV;}
	//return the number of edges in the graph
	int e() const {return nE;}
	//set an edge from <v> to <w> weighted <weight>
	void setEdge(int v, int w, double weight);
	//delete the edge from <v> to <w>
	void delEdge(int v, int w);
	//return true only if there is an edge from <v> to <w>
	bool hasEdge(int v, int w) const;
	//return the weight of the edge from <v> to <w>
	double weight(int v, int w) const;
	/* return the first in the collection of adjacency vertexes of <v>
	   It should return the number of vertexes in the graph if no vertex is adjacent to <v>
	*/
	int first(int v) const;
	/* return the one next to <w> in the collection of adjacency vertexes of <v>
	   It should return the number of vertexes in the graph if no more vertexes are adjacent to <v>
	*/
	int next(int v, int w) const;
	//set the mark of <v>
	void setMark(int v, char mark);
	//get the mark of <v>
	char getMark(int v) const;
};

#endif // !defined(AFX_MGRAPH_H__CA1C7F68_2639_4E1D_9522_08519854E06E__INCLUDED_)
