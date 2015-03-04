#include "stdafx.h"
#include "pixelshader.h"


PixelShader::PixelShader(DeviceResources* pResources) //: Shader(pResources)
{
	PResources = pResources;
	//m_pResources->DeviceContext->PSSetConstantBuffers(0, 1, &m_pResources > ConstBuffer);
	//m_pResources->DeviceContext->PSSetShader(shaders->PixelShader, nullptr, 0);
}


PixelShader::~PixelShader()
{
}

void PixelShader::Load(char* fileName)
{
	ByteCode = File::ReadFileBytes(fileName);
	auto result = PResources->Device->CreatePixelShader(ByteCode->Bytes, ByteCode->Length, nullptr, &PPixelShader);
}

void PixelShader::Compile(char* file, ShaderVersion version)
{

}