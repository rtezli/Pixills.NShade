#pragma once

#include "includes.h"
#include "screen.h"

class Renderer
{
public:
	Renderer(std::shared_ptr<ID3D11Device> pDevice, std::shared_ptr<Screen> pScreen);
	~Renderer();
public:
	//void CreateDeviceDependentResources();
	//void CreateWindowSizeDependentResources();
	//void ReleaseDeviceDependentResources();

	//void StopRendering();
	//void StartTracking();
	//void StopTracking();
	//void TrackingUpdate(float positionX);

	bool IsTracking() { return m_tracking; }
	bool IsRendering(){ return m_isRendering; }
private:
	HRESULT Initialize();
	HRESULT Render();
	HRESULT CreateSwapChain();
private:
	std::shared_ptr<Screen>					m_pScreen;
	std::shared_ptr<ID3D11Device>			m_pDevice;
	std::shared_ptr<IDXGIDevice>			m_pDXGIDevice;
	std::shared_ptr<IDXGIAdapter>			m_pDXGIAdapter;
	std::shared_ptr<IDXGIFactory1>			m_pDXGIFactory;
	std::shared_ptr<IDXGISwapChain>			m_pSwapChain;
	std::shared_ptr<ID3D11Texture2D>		m_pRenderBuffer;

	std::shared_ptr<ID3D11InputLayout>		m_inputLayout;
	std::shared_ptr<ID3D11Buffer>			m_vertexBuffer;
	std::shared_ptr<ID3D11Buffer>			m_indexBuffer;
	std::shared_ptr<ID3D11VertexShader>		m_vertexShader;
	std::shared_ptr<ID3D11PixelShader>		m_pixelShader;
	std::shared_ptr<ID3D11Buffer>			m_constantBuffer;

	std::shared_ptr<ID3D11RenderTargetView>	m_pRenderTarget;
	std::shared_ptr<ID3D11DepthStencilView>	m_pDepthStencilView;

	DirectX::XMFLOAT4X4						m_WorldMatrix;
	DirectX::XMFLOAT4X4						m_ViewMatrix;
	DirectX::XMFLOAT4X4						m_ProjectionMatrix;

	CHAR*									m_standardVertexShader = "PixelShader.cso";
	CHAR*									m_standardPixelShader = "VertexShader.cso";

	INT32									m_ScreenWidth;
	INT32									m_ScreenHeight;

	bool									m_isRendering;
	bool									m_loadingComplete;
	bool									m_tracking;


};
