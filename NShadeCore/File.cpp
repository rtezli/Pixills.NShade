#include "stdafx.h"
#include "file.h"

using namespace std;

struct FileBytes* File::ReadFileBytes(CHAR *fileName)
{
	fstream stream;
	FileBytes* result = (FileBytes*)malloc(sizeof(FileBytes));

	stream.open(fileName, ios::in | ios::binary);
	if (stream.good())
	{
		stream.seekg(0, ios::end);
		SIZE_T size = size_t(stream.tellg());
		CHAR* bytes = new char[size];
		stream.seekg(0, ios::beg);
		stream.read(bytes, size);
		stream.close();

		result->Bytes = bytes;
		result->Length = size;

		return result;
	}
	else
	{
		return result;
	}
}

struct FileBytes* File::ReadFileBytes(LPCWSTR fileName)
{
	fstream stream;
	FileBytes* result = (FileBytes*)malloc(sizeof(FileBytes));

	stream.open(fileName, ios::in | ios::binary);
	if (stream.good())
	{
		stream.seekg(0, ios::end);
		SIZE_T size = size_t(stream.tellg());
		CHAR* bytes = new char[size];
		stream.seekg(0, ios::beg);
		stream.read(bytes, size);
		stream.close();

		result->Bytes = bytes;
		result->Length = size;

		return result;
	}
	else
	{
		return result;
	}
}

vector<string> File::ReadFileLines(CHAR *fileName)
{
	ifstream stream(fileName);

	vector<string> lines;
	lines.clear();

	if (stream.good())
	{
		string line;
		while (!(stream.eof()))
		{	
			getline(stream, line);
			lines.push_back(line);
		}
	}
	return lines;
}