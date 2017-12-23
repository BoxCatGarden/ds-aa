// Graph.h: interface for the Graph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__735DDB55_690D_4704_B8C7_63CDED3B6D88__INCLUDED_)
#define AFX_GRAPH_H__735DDB55_690D_4704_B8C7_63CDED3B6D88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Graph  
{
public:
	Graph() {}
	virtual ~Graph() {}

	//return the number of vertexes in the graph
	virtual int n() const = 0;
	//return the number of edges in the graph
	virtual int e() const = 0;
	//set an edge from <v> to <w> weighted <weight>
	virtual void setEdge(int v, int w, double weight) = 0;
	//delete the edge from <v> to <w>
	virtual void delEdge(int v, int w) = 0;
	//return true only if there is an edge from <v> to <w>
	virtual bool hasEdge(int v, int w) const = 0;
	//return the weight of the edge from <v> to <w>
	virtual double weight(int v, int w) const = 0;
	/* return the first in the collection of adjacency vertexes of <v>
	   It should return the number of vertexes in the graph if no vertex is adjacent to <v>
	*/
	virtual int first(int v) const = 0;
	/* return the one next to <w> in the collection of adjacency vertexes of <v>
	   It should return the number of vertexes in the graph if no more vertexes are adjacent to <v>
	*/
	virtual int next(int v, int w) const = 0;
	//set the mark of <v>
	virtual void setMark(int v, char mark) = 0;
	//get the mark of <v>
	virtual char getMark(int v) const = 0;
};

#endif // !defined(AFX_GRAPH_H__735DDB55_690D_4704_B8C7_63CDED3B6D88__INCLUDED_)
