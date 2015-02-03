#include "stdafx.h"
#include "file.h"

File::File()
{
}


File::~File()
{


}

std::shared_ptr<CHAR> File::ReadFileBytes(CHAR* fileName)
{
	std::ifstream stream(fileName, std::ios::binary | std::ios::ate);
	auto length = stream.tellg();
	CHAR buffer[1024];
	stream.seekg(0, std::ios::beg);
	stream.read(&buffer[0], length);
	stream.close();
	return std::shared_ptr<CHAR>(buffer);
}