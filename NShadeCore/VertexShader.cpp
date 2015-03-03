#include "stdafx.h"
#include "vertexshader.h"

VertexShader::VertexShader(DeviceResources* pResources) : Shader(pResources)
{
	m_pResources = pResources;
}


VertexShader::~VertexShader()
{

}

HRESULT VertexShader::Set()
{
	//m_pResources->DeviceContext->VSSetConstantBuffers(0, 1, &m_pResources->ConstBuffer);
	//m_pResources->DeviceContext->VSSetShader(shaders->VertexShader, nullptr, 0);
	return 0;
}