#pragma once

#include "common.h"
#include "shader.h"

namespace PhongShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
		XMFLOAT3 Normal;
		XMFLOAT4 AmbientColorIntensity;
		XMFLOAT4 LightPositionIntensity;
		XMFLOAT4 EyePosition;
	};

	static const D3D11_INPUT_ELEMENT_DESC InputDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex position
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Model color
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Normal vector

		{ "COLOR",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Ambient light color where w is intensity
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }  // Point light position w is intensity
	};

	class PhongVertexShader : public Shader
	{
	public:
		PhongVertexShader(DeviceResources* pResources);
		~PhongVertexShader();
	};
}