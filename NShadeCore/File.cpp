#include "stdafx.h"
#include "file.h"

File::File()
{
}


File::~File()
{


}

struct FILE_BYTE_RESULT* File::ReadFileBytes(CHAR* fileName)
{
	FILE_BYTE_RESULT* result = new FILE_BYTE_RESULT();

	std::ifstream stream(fileName, std::ios::binary | std::ios::ate);
	stream.seekg(0, std::ios::end);
	auto size = stream.tellg();
	CHAR* buffer = (CHAR *)malloc(size);
	
	stream.seekg(0, std::ios::beg);
	stream.read(&buffer[0], size);

	result->Length = size;
	result->FileBytes = buffer;

	stream.close();
	return result;
}