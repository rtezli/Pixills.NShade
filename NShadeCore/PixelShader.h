#pragma once

#include "common.h"
#include "shader.h"

class PixelShader //: public Shader
{
public:
	PixelShader(DeviceResources* pResources);
	~PixelShader();
	ID3D11PixelShader*	const Shader(){ return PPixelShader; }
protected:
	void				Load(char* file);
	void				Compile(char* file, ShaderVersion version);
	DeviceResources*	PResources;
	FileBytes*			PByteCode;
	ID3D11PixelShader*  PPixelShader;
	FileBytes*			ByteCode;
};

