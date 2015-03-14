#pragma once
#pragma warning( disable : 4244 )

#include "vector"
#include "fstream"
#include "string"

using namespace std;

class File
{
public:
    static struct FileBytes*    ReadFileBytes(char *fileName);
    static struct FileBytes*    ReadFileBytes(wchar_t *fileName);
    static vector<string>       ReadFileLines(char *fileName);
};

struct FileBytes
{
    char*   Bytes;
    long    Length;
};
