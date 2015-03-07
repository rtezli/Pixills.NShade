#pragma once

#include "common.h"
#include "shader.h"

class PixelShader
{
public:
	PixelShader(DeviceResources* pResources);

	ID3D11PixelShader*	const Shader(){ return PPixelShader; }
protected:
	void				Load(char* file);
	void				Compile(char* file, ShaderVersion version);
	DeviceResources*	PResources;
	FileBytes*			PByteCode;
	ID3D11PixelShader*  PPixelShader;
	FileBytes*			ByteCode;
};

