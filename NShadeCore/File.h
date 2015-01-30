#pragma once

#include "Export.h"
#include <iostream>
#include <fstream>
#include <ppltasks.h>

using namespace std;

namespace NShade{
	class File
	{
	public:
		File();
		~File();

	public:
		std::vector<CHAR> NShade::File::ReadFileBytes(CHAR* fileName);
	};
}

