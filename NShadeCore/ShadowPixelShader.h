#pragma once

#include "includes.h"
#include "pixelshader.h"

namespace ShadowShader
{

	class ShadowPixelShader : public PixelShader
	{
	public:
		ShadowPixelShader(DeviceResources* pResources);
		~ShadowPixelShader();
	};
}

