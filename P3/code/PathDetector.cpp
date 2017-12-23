// PathDetector.cpp: implementation of the PathDetector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
using namespace std;

#include "PathDetector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PathDetector::~PathDetector() {
	deleteMatrix(_path);
}

void PathDetector::setMap(const MapLoader& _loader) {
	deleteMatrix(_path); _path = NULL, loader = &_loader;
}

void PathDetector::refresh() {
	deleteMatrix(_path); _path = NULL;
}

void PathDetector::detect(const string& origin, const string& destination) {
	const Graph* g = loader->graph();
	int i, j;
	
	/*lazily calculating*/
	if (!_path && g->n()) {
		const int n = g->n();
		double** _distance = newMatrix<double>(n, n);
		_path = newMatrix<int>(n, n);
		/*initialize*/
		for (i = 0; i < n; ++i){
			for (j = 0; j < n; ++j) {
				if (g->hasEdge(i, j)) {
					_distance[i][j] = g->weight(i, j);
					_path[i][j] = i;
				} else {
					_distance[i][j] = INFINITY;
					_path[i][j] = -1;
				}
			}
			_path[i][i] = i;
		}
		/*Floyd*/
		for (int k = 0; k < n; ++k)
			for (i = 0; i < n; ++i)
				for (j = 0; j < n; ++j)
					if (_distance[i][k] != INFINITY && _distance[k][j] != INFINITY
							&& (_distance[i][j] == INFINITY || _distance[i][k] + _distance[k][j] < _distance[i][j])) {
						_distance[i][j] = _distance[i][k] + _distance[k][j];
						_path[i][j] = _path[k][j];
					}
/*
{
	for (int i = 0; i < g->n(); ++i) {
		for (int j = 0; j < g->n(); ++j)
			cout << _path[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
}
*/
		deleteMatrix(_distance);
	}

	/*find the path*/
	i = loader->indexOf(origin);
	j = loader->indexOf(destination);
	if (_path && ~i && ~j && ~_path[i][j]) {
		path = destination;
		distance = 0;
		while (j != i) {
			path = loader->name(_path[i][j]) + "->" + path;
			distance += g->weight(_path[i][j], j);
			j = _path[i][j];
		}
	} else {
		path = "none";
		distance = INFINITY;
	}
}
