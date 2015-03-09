#include "stdafx.h"
#include "texturetarget.h"

TextureTarget::TextureTarget(DeviceResources *resources, TextureSettings *settings)
{
	_deviceResources = resources;
	D3D11_TEXTURE2D_DESC textureDesc;
 
	textureDesc.Width				= settings->Width;
	textureDesc.Height				= settings->Height;
	textureDesc.MipLevels			= settings->MipLevels;
	textureDesc.ArraySize			= settings->ArraySize;
	textureDesc.Format				= settings->Format;
	textureDesc.SampleDesc.Quality	= settings->Quality;
	textureDesc.SampleDesc.Count	= settings->SampleCount;

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	auto result = resources->Device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTexture);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = settings->Dimension;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = resources->Device->CreateRenderTargetView(_renderTargetTexture, &renderTargetViewDesc, &_renderTargetView);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	result = resources->Device->CreateShaderResourceView(_renderTargetTexture, &shaderResourceViewDesc, &_shaderResourceView);
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