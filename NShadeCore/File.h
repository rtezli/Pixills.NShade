#pragma once
#pragma warning( disable : 4244 )

#include "common.h"

class File
{
public:
	File();
	~File();
public:
	static struct FileBytes* ReadFileBytes(CHAR* fileName);
	static struct FileBytes* ReadFileBytes(LPCWSTR fileName);
	static vector<string>	 ReadFileLines(CHAR* fileName);
};

struct FileBytes
{
	char* Bytes;
	long  Length;
};
