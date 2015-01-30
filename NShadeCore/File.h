#pragma once

#include "Export.h"
#include <iostream>
#include <fstream>
#include <ppltasks.h>

using namespace std;


	class File : public IUnknown
	{
	public:
		File();
		~File();

		virtual ULONG __stdcall Release() = 0;
	public:
		std::vector<CHAR> File::ReadFileBytes(CHAR* fileName);
	};


