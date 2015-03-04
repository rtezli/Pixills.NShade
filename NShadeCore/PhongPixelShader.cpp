#include "stdafx.h"

#include "phongpixelshader.h"

using namespace PhongShader;

PhongPixelShader::PhongPixelShader(char * file, DeviceResources* pResources) : PixelShader(pResources)
{
	Load(file);
}

PhongPixelShader::~PhongPixelShader()
{
}