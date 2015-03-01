#include "stdafx.h"

#include "shadowvertexshader.h"

using namespace ShadowShader;

ShadowVertexShader::ShadowVertexShader(DeviceResources* pResources, Light* light) : Shader(pResources)
{
	m_pLight = make_shared<Light>(*light);
}

ShadowVertexShader::~ShadowVertexShader()
{

}

HRESULT Shader::Initialize()
{
	ShaderFilePath = L"../Debug/ShadowVertexShader.cso";
	Type = ShaderType::VS;
	Version = ShaderVersion::V5;

	auto result = Load(ShaderFilePath);
	if (FAILED(result))
	{
		return result;
	}
}