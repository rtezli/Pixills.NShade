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

	m_pDevice->Release();
	m_pDeviceContext->Release();;
	m_pRenderTarget->Release();
	m_pDepthStencilView->Release();
}

HRESULT D3DSystem::InitializeWithWindow(
	INT32 screenWidth,
	INT32 screenHeight,
	BOOL vsync,
	BOOL fullscreen,
	FLOAT screenDepth,
	FLOAT screenNear)
{
	InitializeWindow(screenWidth, screenHeight);
	return 0;
}

HRESULT D3DSystem::InitializeForWindow(
	BOOL vsync,
	HWND* hwnd,
	BOOL fullscreen,
	FLOAT screenDepth,
	FLOAT screenNear)
{
	RECT windowRect;
	auto result = GetWindowRect(*hwnd, &windowRect);

	if (FAILED(result))
	{
		return result;
	}

	m_ScreenWidth = windowRect.right - windowRect.left;
	m_ScreenHeight = windowRect.bottom - windowRect.top;

	m_vsync_enabled = vsync;
	m_Fullscreen = fullscreen;
	m_pWindow = std::shared_ptr<HWND>(hwnd);

	CreateDevice();
	CreateSwapChain();
	CreateCamera();
	LoadModels();
	CreateRenderer();

	return 0;
}

HWND D3DSystem::Create3DWindow(
	INT32 screenWidth,
	INT32 screenHeight,
	BOOL vsync,
	HWND* hwnd,
	BOOL fullscreen)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

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
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
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

	m_pWindow = std::shared_ptr<HWND>(&handle);
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


	ID3D11Device* device = 0;
	ID3D11DeviceContext* context = 0;

	createResult = D3D11CreateDevice(
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

	m_pDevice = std::shared_ptr<ID3D11Device>(device);
	m_pDeviceContext = std::shared_ptr<ID3D11DeviceContext>(context);

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
	IDXGIDevice* dxgiDevice = 0;
	IDXGIAdapter* dxgiAdapter = 0;
	IDXGIFactory1* dxgiFactory = 0;

	auto result = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter);
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), (void **)&dxgiFactory);

	m_pDXGIDevice = std::shared_ptr<IDXGIDevice>(dxgiDevice);
	m_pDXGIAdapter = std::shared_ptr<IDXGIAdapter>(dxgiAdapter);
	m_pDXGIFactory = std::shared_ptr<IDXGIFactory1>(dxgiFactory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = m_ScreenWidth;
	swapChainDesc.BufferDesc.Height = m_ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		//swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		//swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = (*m_pWindow);

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (m_Fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	auto device = m_pDevice.get();
	auto swapChain = m_pSwapChain.get();
	result = m_pDXGIFactory->CreateSwapChain(device, &swapChainDesc, &swapChain);

	return 0;
}

HRESULT D3DSystem::CreateCamera()
{
	return 0;
}

HRESULT D3DSystem::LoadModels()
{
	m_pModel = std::shared_ptr<Model>(new Model());
	auto device = m_pDevice.get();
	auto cube = m_pModel->Cube;
	auto size = cube.size();
	auto result = m_pModel->Initialize(device, &cube, size);
	return 0;
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
	ID3D11RenderTargetView* targetView = 0;
	//auto buffer = m_pRenderBuffer.get();
	//auto target = m_pRenderTarget.get();
	auto chain = m_pSwapChain.get();
	chain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	//m_pRenderBuffer = std::shared_ptr<ID3D11Texture2D>(backBuffer);

	m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &targetView);
	chain->Present(1, 0);
}

VOID D3DSystem::Destroy()
{
	//m_pDeviceContext;
	//m_pDXGIFactory;
	//m_pSwapChain;
	//m_pRenderTarget;
	//m_pDepthStencilView;
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
