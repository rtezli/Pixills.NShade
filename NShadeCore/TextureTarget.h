#pragma once

#include "common.h"

class TextureTarget
{
public:
	TextureTarget(DeviceResources *pResources);
	~TextureTarget();
	HRESULT Initialize();
	HRESULT								SetRenderTarget(ID3D11DepthStencilView*  depthStencil);
	HRESULT								ClearRenderTarget(ID3D11DepthStencilView* depthStencil);
	ID3D11ShaderResourceView*			GetShaderResourceView();
	DeviceResources*			const	Resources(){ return m_pResources; }
	ID3D11ShaderResourceView*	const	View(){ return m_shaderResourceView; }
private:
	DeviceResources*			m_pResources;
	ID3D11Texture2D*			m_renderTargetTexture;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11ShaderResourceView*	m_shaderResourceView;
};

