#include "stdafx.h"
#include "d3dsystem.h"

D3DSystem::D3DSystem()
{
}

D3DSystem::~D3DSystem()
{
	m_pRenderer.reset();
	m_pCamera.reset();
	m_pInputDevices.reset();
	m_pModel.reset();

	delete m_pDeviceResources;
}

HRESULT D3DSystem::InitializeWithWindow(
	int screenWidth,
	int screenHeight,
	bool vsync,
	bool fullscreen)
{
	auto result = InitializeWindow(screenWidth, screenHeight);
	if (FAILED(result))
	{
		return result;
	}
	return InitializeForWindow(vsync, m_pWindowHandle, fullscreen);
}

HRESULT D3DSystem::InitializeWindow(int screenWidth, int screenHeight)
{
	HINSTANCE hInstance = 0;
	HWND handle = 0;

	handle = CreateWindow(
		L"",
		L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		screenWidth,
		CW_USEDEFAULT,
		screenHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!handle)
	{
		return false;
	}
	m_pWindowHandle = &handle;
	return true;
}

HRESULT D3DSystem::InitializeForWindow(
	bool vsync,
	HWND* hwnd,
	bool fullscreen)
{
	m_pWindowHandle = hwnd;
	m_vSync = vsync;
	m_fullScreen = fullscreen;

	RECT rect;
	
	GetWindowRect(*hwnd, &rect);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	m_viewportWidth = GetSystemMetrics(SM_CXSCREEN);
	m_viewportHeight = GetSystemMetrics(SM_CYSCREEN);

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - m_viewportWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - m_viewportHeight) / 2;

	// ShowCursor(false);

	return Initialize();
}

HRESULT D3DSystem::Initialize()
{
	auto result = CreateDevice();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateRenderer();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateCamera();
	if (FAILED(result))
	{
		return result;
	}

	result = LoadModels();
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT D3DSystem::CreateDevice()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

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

	ID3D11Device* device = 0;
	ID3D11DeviceContext* context = 0;

	auto createResult = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&device,
		&m_D3dFeatureLevel,
		&context);

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
			&device,
			&m_D3dFeatureLevel,
			&context);
	}

	m_pDeviceResources = new DeviceResources(device, context);

	m_pDeviceResources->WindowHandle = m_pWindowHandle;
	m_pDeviceResources->FullScreen = m_fullScreen;
	m_pDeviceResources->VSync = m_vSync;
	m_pDeviceResources->ScreenWidth = m_viewportWidth;
	m_pDeviceResources->ScreenHeight = m_viewportHeight;
	m_pDeviceResources->NearZ = 0.0f;
	m_pDeviceResources->FarZ = 1000.0f;
 
	return createResult;
}

HRESULT D3DSystem::CreateCamera()
{
	m_pCamera = shared_ptr<Camera>(new Camera(m_pDeviceResources));
	m_pCamera->Initialize();
	return 0;
}

HRESULT D3DSystem::LoadModels()
{
	m_pModel = shared_ptr<Model>(new Model(m_pDeviceResources));
	auto result = m_pModel->Initialize();
	return 0;
}

HRESULT D3DSystem::CreateRenderer()
{
	m_pRenderer = shared_ptr<Renderer>(new Renderer(m_pDeviceResources, true));
	return m_pRenderer->Initialize();
}

void D3DSystem::Render()
{
	Reset();
	m_pRenderer->Render();
}

void D3DSystem::Reset()
{
	auto context = m_pDeviceResources->DeviceContext;

	// Reset the viewport to target the whole screen.
	auto viewport = m_pDeviceResources->ViewPort;
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_pDeviceResources->RenderTargetView };
	context->OMSetRenderTargets(1, targets, m_pDeviceResources->DepthStencilView);
	context->ClearRenderTargetView(m_pDeviceResources->RenderTargetView, m_pDeviceResources->DefaultColor);
	context->ClearDepthStencilView(m_pDeviceResources->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		//return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
	return 0;
}