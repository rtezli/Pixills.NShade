#include "stdafx.h"

#include "phongvertexshader.h"

using namespace PhongShader;

PhongVertexShader::PhongVertexShader(DeviceResources* pResources) : VertexShader(pResources)
{
	ByteWidth = sizeof(PhongShader::InputLayout);
	PResources->Device->CreateInputLayout(InputDescription, ARRAYSIZE(InputDescription), ByteCode->Bytes, ByteCode->Length, &PInputLayout);
}

PhongVertexShader::~PhongVertexShader()
{
}