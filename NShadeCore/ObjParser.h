#pragma once
#pragma warning(disable:4996)

#include "windows.h"
#include "export.h"
#include "res.h"
#include "file.h"
#include "vector"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

using namespace std;

EXTERN class API ObjParser
{
public:
    static HRESULT Parse(vector<NVertex> *vertices, vector<unsigned int> *indices, char *fileName);
};

#pragma warning(restore:4996)