#pragma once

#include "common.h"
#include "pixelshader.h"

namespace PhongShader
{
	class PhongPixelShader : public PixelShader
	{
	public:
		PhongPixelShader(CHAR *file, DeviceResources* pResources);
		~PhongPixelShader();
	};
}
