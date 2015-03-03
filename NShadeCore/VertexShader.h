#pragma once

#include "includes.h"
#include "shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(DeviceResources* pResources);
	~VertexShader();
	HRESULT Set();
private:
	DeviceResources*		m_pResources;
};
