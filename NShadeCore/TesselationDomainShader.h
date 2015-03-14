#pragma once

#include "res.h"
#include "shader.h"

namespace TesselationShader
{
	struct DomainInputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
		XMFLOAT3 BaryCoord;
	};

	class TesselationDomainShader : Shader
	{

	};
}

