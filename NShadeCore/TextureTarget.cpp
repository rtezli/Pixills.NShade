#include "stdafx.h"
#include "texturetarget.h"

TextureTarget::TextureTarget()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Setup the render target texture description.
	textureDesc.Width = _deviceResources->ViewPort->Width;
	textureDesc.Height = _deviceResources->ViewPort->Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//textureDesc.SampleDesc.Quality = m_pRenderingQuality->Quality;
	//textureDesc.SampleDesc.Count = Resources()->RenderQuality->SampleCount;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = _deviceResources->Device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTexture);

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = _deviceResources->Device->CreateRenderTargetView(_renderTargetTexture, &renderTargetViewDesc, &_renderTargetView);

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = _deviceResources->Device->CreateShaderResourceView(_renderTargetTexture, &shaderResourceViewDesc, &_shaderResourceView);
}

VOID TextureTarget::SetRenderTarget(ID3D11DepthStencilView *depthStencilView)
{
	_deviceResources->DeviceContext->OMSetRenderTargets(1, &_renderTargetView, depthStencilView);
}

VOID TextureTarget::ClearRenderTarget(ID3D11DepthStencilView *depthStencilView)
{
	const FLOAT color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_deviceResources->DeviceContext->ClearRenderTargetView(_renderTargetView, color);
	_deviceResources->DeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}