#pragma once

#include "export.h"
#include "iostream"
#include "fstream"
#include "memory"
#include "ppltasks.h"

using namespace std;

class File
{
public:
	File();
	~File();
public:
	std::shared_ptr<CHAR> File::ReadFileBytes(CHAR* fileName);
};


