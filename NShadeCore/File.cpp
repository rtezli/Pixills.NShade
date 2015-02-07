#include "stdafx.h"
#include "file.h"

using namespace std;

File::File()
{
}


File::~File()
{


}

struct FILE_BYTE_RESULT* File::ReadFileBytes(CHAR* fileName)
{
	fstream stream;
	FILE_BYTE_RESULT* result;
	ZeroMemory(&result, sizeof(FILE_BYTE_RESULT));

	stream.open(fileName, ios::in | ios::binary);
	if (stream.good())
	{
		stream.seekg(0, ios::end);
		SIZE_T size = size_t(stream.tellg());
		CHAR* bytes = new char[size];
		stream.seekg(0, ios::beg);
		stream.read(bytes, size);
		stream.close();

		result->FileBytes = bytes;
		result->Length = size;

		return result;
	}
	else
	{
		return result;
	}
}

struct FILE_BYTE_RESULT* File::ReadFileBytes(LPCWSTR fileName)
{
	fstream stream;
	FILE_BYTE_RESULT* result = (FILE_BYTE_RESULT *)malloc(sizeof(FILE_BYTE_RESULT));

	stream.open(fileName, ios::in | ios::binary);
	if (stream.good())
	{
		stream.seekg(0, ios::end);
		SIZE_T size = size_t(stream.tellg());
		CHAR* bytes = new char[size];
		stream.seekg(0, ios::beg);
		stream.read(bytes, size);
		stream.close();

		result->FileBytes = bytes;
		result->Length = size;

		return result;
	}
	else
	{
		return result;
	}
}