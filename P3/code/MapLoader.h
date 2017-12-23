// MapLoader.h: interface for the MapLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPLOADER_H__36BAA8C3_3D38_49D3_B0CB_5FB6E2BF5A08__INCLUDED_)
#define AFX_MAPLOADER_H__36BAA8C3_3D38_49D3_B0CB_5FB6E2BF5A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MGraph.h"

class MapLoader  
{
	Graph* g;  //the corresponding graph
	string* key;  //the table of place names
	int n;  //the number of the places in the map
	string _unit;  //the length unit of the map
public:
	MapLoader():g(NULL),key(NULL),n(0) {}
	virtual ~MapLoader() {delete g; delete[] key;}

	/* filename: the name of the file that contains the information about a map

	   return: true if the map was loaded successfully; else false
	
	   Note: It will generate the corresponding graph based on the map. You can use graph() to get it.
	   It will also create a mapping table between place names and its ID. Use indexOf(const string&) to look up IDs in it.
	   The file should be in format:
	   [n] [unit]
	   [places:p1 p2 ...]
	   [matrix:
	   x  x  ...
	   x  x  ...
	   .........]
	*/
	bool load(const char* filename);

	/* placename: the name of the place of which the ID is looked up

	   return: the corresponding ID of placename
	*/
	int indexOf(const string& placename) const;

	/* i: the index of the expected place name

	   return: the place name of index <i>

	   Note: It doesn't hold exceptions such as invalid <i>, or NUll [key], since there's no help to hold those
	   but to hide some logical errors.
	*/
	const string& name(int i) const {return key[i];}

	//return the length unit of the map
	const string& unit() const {return _unit;}

	//return the corresponding graph based on the map
	const Graph* graph() const {return g;}

	//return the number of the places in the map
	int size() const {return n;}
};

#endif // !defined(AFX_MAPLOADER_H__36BAA8C3_3D38_49D3_B0CB_5FB6E2BF5A08__INCLUDED_)
