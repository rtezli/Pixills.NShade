#pragma once

#include "includes.h"
#include "shader.h"

namespace TesselationShader
{
	struct HullInputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT4 ColorWithAlpha;
		XMFLOAT3 BaryCoord;
	};

	class TesselationHullShader : public Shader
	{
	public:
		TesselationHullShader(DeviceResources* pResources);
		~TesselationHullShader();
	};
}

