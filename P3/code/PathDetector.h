// PathDetector.h: interface for the PathDetector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHDETECTOR_H__78F12CE6_1A70_4E7A_9272_CC46E151B015__INCLUDED_)
#define AFX_PATHDETECTOR_H__78F12CE6_1A70_4E7A_9272_CC46E151B015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapLoader.h"

#define INFINITY -1.0

class PathDetector  
{
	int** _path;  //path matrix
	const MapLoader* loader;  //the map loader holding the graph
public:
	string path;  //the resulting path of using detect(const string&,const string&)
	double distance;  //the length of the resulting path; INFINITY if the path is inexistent

	PathDetector():_path(NULL),loader(NULL) {}
	virtual ~PathDetector();

	/*set the map used for detect(const string&,const string&)*/
	void setMap(const MapLoader& loader);

	/* origin: the origin of the path to detect
	   destination: the destination of the path to detect

	   Before: ensure that you've set a loader through setMap(const MapLoader&)

	   Note: After having called it, you can get the result by direct fetching [path] and [distance].
	   If there is no path from <origin> to <destination>, [path] will be 'none' and [distance] will be INFINITY.
	*/
	void detect(const string& origin, const string& destination);

	/*if you reload the map and don't re-setMap(), use it to just mark that the cache of paths needs refreshing*/
	void refresh();
};

#endif // !defined(AFX_PATHDETECTOR_H__78F12CE6_1A70_4E7A_9272_CC46E151B015__INCLUDED_)
