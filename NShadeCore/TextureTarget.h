#pragma once

#include "common.h"

class TextureTarget
{
public:
	TextureTarget();
public:
	VOID								SetRenderTarget(ID3D11DepthStencilView *depthStencil);
	VOID								ClearRenderTarget(ID3D11DepthStencilView *depthStencil);
	ID3D11ShaderResourceView*			const View(){ return _shaderResourceView; }
private:
	DeviceResources*					_deviceResources;
	ID3D11Texture2D*					_renderTargetTexture;
	ID3D11RenderTargetView*				_renderTargetView;
	ID3D11ShaderResourceView*			_shaderResourceView;
};

