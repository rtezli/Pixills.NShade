#pragma once

#include "includes.h"

class File
{
public:
	File();
	~File();
public:
	std::shared_ptr<CHAR> File::ReadFileBytes(CHAR* fileName);
};


