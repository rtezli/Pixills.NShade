#pragma once
#pragma warning(disable:4996)

#include "common.h"

EXTERN class API ObjParser
{
public:
	static HRESULT Parse(vector<NVertex>* vertices, vector<UINT>* indices, CHAR* fileName);
};

#pragma warning(restore:4996) 