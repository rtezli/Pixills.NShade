#include "stdafx.h"
#include "d3dsystem.h"

NShade::D3DSystem::D3DSystem()
{
}

NShade::D3DSystem::~D3DSystem()
{
}

HRESULT NShade::D3DSystem::InitializeWithWindow(
	int screenWidth,
	int screenHeight,
	bool vsync,
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	HWND windowHandle = 0;
	Initialize(screenWidth, screenHeight, vsync, windowHandle, fullscreen, screenDepth, screenNear);
	return 0;
}

HRESULT NShade::D3DSystem::Initialize(
	int screenWidth,
	int screenHeight,
	bool vsync,
	HWND hwnd,
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	CreateDevice();
	CreateSwapChain();
	CreateCamera();
	LoadModels();
	CreateRenderer();

	return 0;
}

HWND NShade::D3DSystem::Create3DWindow(int screenWidth,
	int screenHeight,
	bool vsync,
	HWND hwnd,
	bool fullscreen)
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateDevice()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	HRESULT createResult = 0;
	
	

	createResult = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&m_pDevice,
		&m_D3dFeatureLevel,
		&m_pDeviceContext);

	if (FAILED(createResult))
	{
		createResult = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
			0,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&m_pDevice,
			&m_D3dFeatureLevel,
			&m_pDeviceContext);
	}

	return createResult;
}

HRESULT NShade::D3DSystem::SetCamera(XMVECTOR position, XMVECTOR direction, UINT16 viewAngle)
{
	return 0;
}

HRESULT NShade::D3DSystem::SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength)
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateSwapChain()
{
	HRESULT result = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

	result = m_pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
	return 0;
}

HRESULT NShade::D3DSystem::CreateCamera()
{
	return 0;
}

HRESULT NShade::D3DSystem::LoadModels()
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateRenderer()
{
	return 0;
}

HRESULT NShade::D3DSystem::ApplyShaders()
{
	return 0;
}

HRESULT NShade::D3DSystem::Render()
{
	ID3D11Texture2D* backBuffer = 0;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTarget);
 
	// TODO : limit FPS to a value
	while (m_pRenderer->IsRendering())
	{
		m_pSwapChain->Present(1, 0);
	}
	return 0;
}
