#pragma once

#include "common.h"
#include "shader.h"

class PixelShader //: public Shader
{
public:
	PixelShader(DeviceResources* pResources);
	~PixelShader();
private:
	DeviceResources*	PResources;
	FileBytes*			ByteCode;
};

