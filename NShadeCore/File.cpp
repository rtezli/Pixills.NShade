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

	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	std::ifstream stream(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (stream)
	{
		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		CHAR* buffer;
		ZeroMemory(&buffer, sizeof(CHAR));

		stream.seekg(0, std::ios::beg);
		stream.read(&buffer[0], size);

		result->Length = size;
		result->FileBytes = buffer;

		stream.close();
	}
	return result;
}

struct FILE_BYTE_RESULT* File::ReadFileBytes(LPCWSTR fileName)
{
	FILE_BYTE_RESULT* result = new FILE_BYTE_RESULT();

	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	std::ifstream stream(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (stream)
	{
		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		CHAR* buffer = (CHAR *)malloc(sizeof(CHAR) * size);

		stream.seekg(0, std::ios::beg);
		stream.read(&buffer[0], size);

		result->Length = size;
		result->FileBytes = buffer;

		stream.close();
	}
	return result;
}