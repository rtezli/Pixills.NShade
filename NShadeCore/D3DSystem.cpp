#include "stdafx.h"
#include "d3dsystem.h"

D3DSystem::D3DSystem()
{
}

D3DSystem::~D3DSystem()
{
}

HRESULT D3DSystem::InitializeWithWindow(
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

HRESULT D3DSystem::Initialize(
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

HWND D3DSystem::Create3DWindow(int screenWidth,
	int screenHeight,
	bool vsync,
	HWND hwnd,
	bool fullscreen)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	//ApplicationHandle = this;

	// Get the instance of this application.
	auto applicatioInstance = GetModuleHandle(NULL);

	// Give the application a name.
	auto applicationName = L"Simple Rendering Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = applicatioInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (fullscreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	RECT rect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	auto handle =
		CreateWindowEx(
		WS_EX_APPWINDOW,
		applicationName,
		applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX,
		posY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		applicatioInstance,
		NULL);

	ShowWindow(handle, SW_SHOW);
	SetForegroundWindow(handle);
	SetFocus(handle);

	// Hide the mouse cursor.
	// ShowCursor(false);

	return handle;
}

HRESULT InitializeWindow(int& screenWidth, int& screenHeight)
{
	return 0;
}

HRESULT D3DSystem::CreateDevice()
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

HRESULT D3DSystem::SetCamera(XMVECTOR position, XMVECTOR direction, UINT16 viewAngle)
{
	return 0;
}

HRESULT D3DSystem::SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength)
{
	return 0;
}

HRESULT D3DSystem::CreateSwapChain()
{
	HRESULT result = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

	result = m_pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
	return 0;
}

HRESULT D3DSystem::CreateCamera()
{
	return 0;
}

HRESULT D3DSystem::LoadModels()
{
	//auto cube = m_pModel->Cube;
	auto result = m_pModel->Initialize(m_pDevice, NULL);
	return result;
}

HRESULT D3DSystem::CreateRenderer()
{
	return 0;
}

HRESULT D3DSystem::ApplyShaders()
{
	return 0;
}

VOID D3DSystem::Render()
{
	ID3D11Texture2D* backBuffer = 0;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTarget);

	// TODO : limit FPS to a value
	while (m_pRenderer->IsRendering())
	{
		m_pSwapChain->Present(1, 0);
	}
}

VOID D3DSystem::Destroy()
{
	if (m_pCamera)
	{
		DESTROY(m_pDeviceContext);
		DESTROY(m_pDXGIFactory);
		DESTROY(m_pSwapChain);
		DESTROY(m_pRenderTarget);
		DESTROY(m_pDepthStencilView);
	}
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
