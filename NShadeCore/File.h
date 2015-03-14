#pragma once
#pragma warning( disable : 4244 )

#include "vector"
#include "fstream"
#include "string"
#include "windows.h"

using namespace std;

class File
{
public:
    static struct FileBytes*    ReadFileBytes(char *fileName);
    static struct FileBytes*    ReadFileBytes(LPCWSTR fileName);
    static vector<string>       ReadFileLines(char *fileName);
};

struct FileBytes
{
    char*   Bytes;
    long    Length;
};
