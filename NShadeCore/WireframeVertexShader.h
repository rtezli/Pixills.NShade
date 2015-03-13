#pragma once

#include "common.h"
#include "shader.h"

namespace WireframeShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
		XMFLOAT3 BaryCoord;
	};

	class WireframeVertexShader : public Shader
	{

	};
}
