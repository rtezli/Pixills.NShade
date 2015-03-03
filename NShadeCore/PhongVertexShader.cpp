#include "stdafx.h"

#include "phongvertexshader.h"

using namespace PhongShader;

PhongVertexShader::PhongVertexShader(DeviceResources* pResources) : VertexShader(pResources)
{
}

PhongVertexShader::~PhongVertexShader()
{
}

HRESULT PhongVertexShader::SetVertices(vector<nshade::Vertex> vertices)
{
	//m_pResources->DeviceContext->VSSetConstantBuffers(0, 1, &m_pResources->ConstBuffer);
	//m_pResources->DeviceContext->VSSetShader(shaders->VertexShader, nullptr, 0);
	return 0;
}