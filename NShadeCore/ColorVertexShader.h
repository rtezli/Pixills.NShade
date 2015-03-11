#pragma once

#include "includes.h"
#include "shader.h"

namespace ColorShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
	};

	class ColorVertexShader : public Shader
	{
	public:
		ColorVertexShader(DeviceResources* pResources);
		~ColorVertexShader();
	};
}