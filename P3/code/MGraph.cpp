// MGraph.cpp: implementation of the MGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MGraph.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MGraph::MGraph(int vertexNum)
:adjMatrix(NULL), mark(NULL), nV(vertexNum), nE(0) {
	if (vertexNum) {
		adjMatrix = newMatrix<double>(vertexNum, vertexNum);
		for (int i = 0, j; i < vertexNum; ++i)
			for (j = 0; j < vertexNum; ++j)
				adjMatrix[i][j] = 0;  //initialize
		mark = new char[vertexNum]; memset(mark, 0, vertexNum);
	}
}

MGraph::~MGraph() {
	deleteMatrix(adjMatrix);
	delete[] mark;
}

void MGraph::setEdge(int v, int w, double weight) {
	if (!adjMatrix[v][w]) ++nE;
	adjMatrix[v][w] = weight;
}

bool MGraph::hasEdge(int v, int w) const {
	return !!adjMatrix[v][w];
}

void MGraph::delEdge(int v, int w) {
	if (adjMatrix[v][w]) {
		--nE;
		adjMatrix[v][w] = 0;
	}
}

double MGraph::weight(int v, int w) const {
	return adjMatrix[v][w];
}

int MGraph::first(int v) const {
	int w = 0;
	while (w < nV && !adjMatrix[v][w]) ++w;
	return w;
}

int MGraph::next(int v, int w) const {
	++w;
	while (w < nV && !adjMatrix[v][w]) ++w;
	return w;
}

void MGraph::setMark(int v, char _mark) {
	mark[v] = _mark;
}

char MGraph::getMark(int v) const {
	return mark[v];
}
