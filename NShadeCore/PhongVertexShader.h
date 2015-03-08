#pragma once

#include "common.h"
#include "vertexshader.h"

namespace PhongShader
{
	struct ExtraData
	{
		XMFLOAT4 AmbientColorIntensity;
		XMFLOAT4 LightPositionIntensity;
	};

	static const D3D11_INPUT_ELEMENT_DESC InputDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex position
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Model color
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Normal vector
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }  // UVs
	};

	class PhongVertexShader : public VertexShader
	{
	public:
		PhongVertexShader(CHAR *file, DeviceResources *pResources);
		~PhongVertexShader();
	};
}