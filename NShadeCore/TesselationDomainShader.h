#pragma once

#include "common.h"
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
	public:
		TesselationDomainShader(DeviceResources* pResources);
		~TesselationDomainShader();
	};
}

