#pragma once

#include "common.h"
#include "shader.h"

namespace PhongShader
{
	class PhongPixelSHader : public Shader
	{
	public:
		PhongPixelSHader(DeviceResources *resources);
		~PhongPixelSHader();
	};
}
