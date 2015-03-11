#pragma once

#include "includes.h"
#include "shader.h"

namespace WireframeShader
{
	class WireframePixelShader : public Shader
	{
	public:
		WireframePixelShader(DeviceResources* pResources);
		~WireframePixelShader();
	};
}
