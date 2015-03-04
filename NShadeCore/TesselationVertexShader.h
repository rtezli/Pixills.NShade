#pragma once

#include "common.h"
#include "vertexshader.h"

namespace TesselationShader
{
	class TesselationVertexShader : public VertexShader
	{
	public:
		TesselationVertexShader(DeviceResources* pResources);
		~TesselationVertexShader();
	};
}

