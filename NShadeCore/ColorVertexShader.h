#pragma once

#include "common.h"
#include "vertexshader.h"

namespace ColorShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
	};

	class ColorVertexShader : public VertexShader
	{
	public:
		ColorVertexShader(DeviceResources* pResources);
		~ColorVertexShader();
	};
}