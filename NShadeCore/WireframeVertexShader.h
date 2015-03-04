#pragma once

#include "common.h"
#include "vertexshader.h"

namespace WireframeShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
		XMFLOAT3 BaryCoord;
	};

	class WireframeVertexShader : public VertexShader
	{
	public:
		WireframeVertexShader(DeviceResources* pResources);
		~WireframeVertexShader();
	};
}
