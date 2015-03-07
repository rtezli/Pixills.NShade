#pragma once
#pragma warning( disable : 4244 )

#include "common.h"

class File
{
public:
	File();
	~File();
public:
	static struct FileBytes* ReadFileBytes(CHAR *pFileName);
	static struct FileBytes* ReadFileBytes(LPCWSTR lpFileName);
	static vector<string>	 ReadFileLines(CHAR *pFileName);
};

struct FileBytes
{
	CHAR* Bytes;
	LONG  Length;
};
