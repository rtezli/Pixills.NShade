#pragma once

#include "common.h"
#include "shader.h"

namespace WireframeShader
{
	class WireframePixelShader : public Shader
	{
	public:
		WireframePixelShader(DeviceResources *resources);
		~WireframePixelShader();
	};
}
