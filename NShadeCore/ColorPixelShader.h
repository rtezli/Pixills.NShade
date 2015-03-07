#pragma once

#include "common.h"

#include "pixelshader.h"

namespace ColorShader
{
	class ColorPixelShader : public PixelShader
	{
	public:
		ColorPixelShader(DeviceResources *pResources);
		~ColorPixelShader();
	};
}
