#pragma once

#include "res.h"
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

	};
}