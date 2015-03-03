#pragma once

#include "includes.h"

#include "pixelshader.h"

namespace ColorShader
{
	class ColorPixelShader : public PixelShader
	{
	public:
		ColorPixelShader(DeviceResources* pResources);
		~ColorPixelShader();
	};
}
