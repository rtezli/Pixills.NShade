#pragma once

#include "includes.h"
#include "shader.h"

class PixelShader : public Shader
{
public:
	PixelShader(DeviceResources* pResources);
	~PixelShader();
	HRESULT Set();
private:
	DeviceResources*		m_pResources;
};

