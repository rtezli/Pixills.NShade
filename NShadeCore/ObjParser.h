#pragma once
#pragma warning( disable : 4996 )

#include "includes.h"

class ObjParser
{
public:
	ObjParser();
	~ObjParser();
public:
	static HRESULT Parse(vector<nshade::Vertex>* vertices, vector<unsigned int>* indices, char* fileName);
};

#pragma warning( restore : 4996 ) 