#pragma once

#include "common.h"
#include "texturetarget.h"
#include "buffertarget.h"

class D3D
{
public:
	/**
	* 
	* SetRenderTarget makes use of OMSetRenderTargets with NULL for pDepthStencilView so it does not bind the depth stencil
	* 
	**/
	static VOID SetRenderTarget(DeviceResources *resources, ID3D11Buffer *buffer, TextureSettings *settings);
	/**
	*
	* SetRenderTarget makes use of OMSetRenderTargets with NULL for pDepthStencilView so it does not bind the depth stencil
	*
	**/
	static VOID SetRenderTarget(DeviceResources *resources, ID3D11Texture2D *texture);
};

