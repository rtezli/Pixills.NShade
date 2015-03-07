#include "stdafx.h"
#include "pixelshader.h"


PixelShader::PixelShader(DeviceResources* pResources)
{
	PResources = pResources;
}

void PixelShader::Load(char* fileName)
{
	ByteCode = File::ReadFileBytes(fileName);
	auto result = PResources->Device->CreatePixelShader(ByteCode->Bytes, ByteCode->Length, NULL, &PPixelShader);
}

void PixelShader::Compile(char* file, ShaderVersion version)
{

}