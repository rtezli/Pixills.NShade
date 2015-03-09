#pragma once

#include "common.h"

class BufferTarget
{
public:
	BufferTarget();
	~BufferTarget();
private:
	ID3D11Buffer*			_bufferTarget;
};

