#include "stdafx.h"

#include "phongvertexshader.h"

using namespace PhongShader;

PhongVertexShader::PhongVertexShader(char * file, DeviceResources* pResources) : VertexShader(pResources)
{
	Load(file);
	ByteWidth = sizeof(PhongShader::InputLayout);
	PResources->Device->CreateInputLayout(InputDescription, ARRAYSIZE(InputDescription), ByteCode->Bytes, ByteCode->Length, &PInputLayout);
}

PhongVertexShader::~PhongVertexShader()
{
}