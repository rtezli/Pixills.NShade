#include "stdafx.h"

#include "phongvertexshader.h"

using namespace PhongShader;

PhongVertexShader::PhongVertexShader(CHAR *file, DeviceResources *pResources) : VertexShader(pResources)
{
	Load(file);
	SetExtraDataSize(sizeof(PhongShader::ExtraData));
	PResources->Device->CreateInputLayout(InputDescription, ARRAYSIZE(InputDescription), PByteCode->Bytes, PByteCode->Length, &PInputLayout);
}
