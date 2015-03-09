#pragma once

#include "common.h"

struct TextureSettings
{
	TextureSettings() :
		Format(DXGI_FORMAT_B8G8R8A8_UNORM),
		Dimension(D3D11_RTV_DIMENSION_TEXTURE2DMS
		) {}

	DXGI_FORMAT Format;
	D3D11_RTV_DIMENSION Dimension;
	UINT Width;
	UINT Height;
	UINT MipLevels;
	UINT ArraySize;
	UINT Quality;
	UINT SampleCount;
};

class TextureTarget
{
public:
	TextureTarget(DeviceResources *resources, TextureSettings *settings);
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

