#pragma once

#include "common.h"
#include "shader.h"

namespace TesselationShader
{
	class TesselationVertexShader : public Shader
	{
	public:
		TesselationVertexShader(DeviceResources *resources);
		~TesselationVertexShader();
	};
}

