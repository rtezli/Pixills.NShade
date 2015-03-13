#pragma once
#pragma warning( disable : 4244 )

#include <fstream>
#include "common.h"

class File
{
public:
    static struct FILE_BYTE_RESULT* ReadFileBytes(CHAR* fileName);
    static struct FILE_BYTE_RESULT* ReadFileBytes(LPCWSTR fileName);
    static vector<string>           ReadFileLines(CHAR* fileName);
};

struct FILE_BYTE_RESULT
{
    CHAR*   FileBytes;
    long    Length;
};
