// MapLoader.cpp: implementation of the MapLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
using namespace std;

#include "MapLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool MapLoader::load(const char* filename) {
	int i, j;
	if (!filename[0]) return false;
	ifstream file(filename);
	if (!file || file.peek() == EOF) return false;
	file >> i;  //read the number of places
	if (!i) return false;
	n = i;
	file >> _unit;  //read the unit of length
	if (_unit == "_") _unit = "";
	delete g; g = new MGraph(n);
	delete[] key; key = new string[n];
	double w;
	//read places
	for (i = 0; i < n; ++i) {
		file >> key[i];
	}
	//read matrix
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j) {
			file >> w;
			g->setEdge(i, j, w);
		}
	return true;
}

int MapLoader::indexOf(const string& placename) const {
	int i;
	for (i = 0; i < n && key[i] != placename; ++i);
	return (i<n)?i:-1;
}
