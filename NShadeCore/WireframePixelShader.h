#pragma once

#include "includes.h"
#include "pixelshader.h"

namespace WireframeShader
{
	class WireframePixelShader : public PixelShader
	{
	public:
		WireframePixelShader(DeviceResources* pResources);
		~WireframePixelShader();
	};
}
