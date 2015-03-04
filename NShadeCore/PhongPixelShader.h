#pragma once

#include "common.h"
#include "pixelshader.h"

namespace PhongShader
{
	class PhongPixelShader : public PixelShader
	{
	public:
		PhongPixelShader(DeviceResources* pResources);
		~PhongPixelShader();
	};
}
