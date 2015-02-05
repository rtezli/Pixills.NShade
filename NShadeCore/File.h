#pragma once
#include <fstream>

#include "includes.h"

class File
{
public:
	File();
	~File();
public:
	static struct FILE_BYTE_RESULT* ReadFileBytes(CHAR* fileName);
	static struct FILE_BYTE_RESULT* ReadFileBytes(LPCWSTR fileName);
};

struct FILE_BYTE_RESULT
{
	CHAR* FileBytes;
	long Length;
};
