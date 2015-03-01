#pragma once

#include "includes.h"
#include "shader.h"

namespace PhongShader
{
	class PhongPixelShader : public Shader
	{
	public:
		PhongPixelShader(DeviceResources* pResources);
		~PhongPixelShader();
	};
}
