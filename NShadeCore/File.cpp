#include "stdafx.h"
#include "File.h"



NShade::File::File()
{
}


NShade::File::~File()
{
	
	
}

std::vector<CHAR> NShade::File::ReadFileBytes(CHAR* fileName)
{
	ifstream stream(fileName, ios::binary | ios::ate);
	std::streamoff length = stream.tellg();
	std::vector<CHAR> result(length);
	stream.seekg(0, ios::beg);
	stream.read(&result[0], length);
	stream.close();

	return result;
}