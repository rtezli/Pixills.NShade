#include "stdafx.h"

#include "phongvertexshader.h"

using namespace PhongShader;

PhongVertexShader::PhongVertexShader(CHAR *file, DeviceResources *deviceResources) : VertexShader(deviceResources)
{
	Load(file);
	_Resources->Device->CreateInputLayout(InputDescription, ARRAYSIZE(InputDescription), _ByteCode->Bytes, _ByteCode->Length, &_InputLayout);
}
