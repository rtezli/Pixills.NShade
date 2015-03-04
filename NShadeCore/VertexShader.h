#pragma once

#include "common.h"
#include "shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(DeviceResources* pResources);
	virtual ~VertexShader();
	virtual HRESULT SetVertices(vector<nshade::Vertex> vertices) = 0;
private:
	DeviceResources*		m_pResources;
};
