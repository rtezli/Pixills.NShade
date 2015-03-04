#pragma once

#include "common.h"
#include "shader.h"

class PixelShader //: public Shader
{
public:
	PixelShader(DeviceResources* pResources);
	~PixelShader();
protected:
	void				Load(char* file);
	void				Compile(char* file, ShaderVersion version);
	DeviceResources*	PResources;
	FileBytes*			PByteCode;
	ID3D11PixelShader*  PPixelShader;
	FileBytes*			ByteCode;
};

