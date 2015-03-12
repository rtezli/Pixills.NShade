#pragma once

#include "common.h"
#include "shader.h"

namespace ColorShader
{
	class ColorPixelShader : public Shader
	{
	public:
		ColorPixelShader(DeviceResources *resources);
		~ColorPixelShader();
	};
}
