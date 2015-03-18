#pragma once

#include "res.h"
#include "node.h"

namespace WireframeShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
		XMFLOAT3 BaryCoord;
	};

	class WireframeVertexShader : public Node
	{

	};
}
