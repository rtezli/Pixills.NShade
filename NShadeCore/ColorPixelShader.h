#pragma once

#include "includes.h"
#include "shader.h"

namespace ColorShader
{
	class ColorPixelShader : public Shader
	{
	public:
		ColorPixelShader(DeviceResources* pResources);
		~ColorPixelShader();
	};
}
