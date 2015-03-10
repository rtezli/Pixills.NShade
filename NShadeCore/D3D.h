#pragma once

#include "common.h"
#include "texturetarget.h"
#include "buffertarget.h"

class D3D
{
public:
	static VOID SetRenderTarget(DeviceResources *resources, ID3D11Buffer *buffer, TextureSettings *settings);
	static VOID SetRenderTarget(DeviceResources *resources, ID3D11Texture2D *texture);
};

