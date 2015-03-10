#pragma once

#include "common.h"
#include "rendertarget.h"

class BufferTarget : public RenderTarget
{
public:
	BufferTarget(DeviceResources *resources);
private:
	ID3D11Buffer*			_bufferTarget;
};

