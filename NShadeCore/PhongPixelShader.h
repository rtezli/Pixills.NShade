#pragma once

#include "includes.h"
#include "shader.h"

namespace PhongShader
{
	class PhongPixelSHader : public Shader
	{
	public:
		PhongPixelSHader(DeviceResources* pResources);
		~PhongPixelSHader();
	};
}
