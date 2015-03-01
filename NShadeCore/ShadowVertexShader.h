#pragma once

#include "includes.h"
#include "shader.h"

namespace ShadowShader
{
	const struct ConstantBuffer
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	const struct  InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT3 TextCoord;
		XMFLOAT3 Normal;
	};

	const D3D11_INPUT_ELEMENT_DESC InputDescription[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	class ShadowVertexShader : Shader
	{
	public:
		ShadowVertexShader(DeviceResources* pResources, Light* light);
		~ShadowVertexShader();
	private:
		shared_ptr<Light>			m_pLight;
	};
}

