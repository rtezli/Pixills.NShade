#pragma once

#include "includes.h"

class File
{
public:
	File();
	~File();
public:
	static struct FILE_BYTE_RESULT* ReadFileBytes(CHAR* fileName);
};

struct FILE_BYTE_RESULT
{
	CHAR* FileBytes;
	long Length;
};
