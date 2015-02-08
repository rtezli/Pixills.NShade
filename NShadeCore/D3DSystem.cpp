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

	result = LoadModels();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateCamera();
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
	return 0;
}

HRESULT D3DSystem::LoadModels()
{
	m_pModel = shared_ptr<Model>(new Model(m_pDeviceResources));
	auto cube = m_pModel->Cube;
	auto size = cube.size();
	auto result = m_pModel->Initialize(&cube, size);
	return 0;
}

HRESULT D3DSystem::CreateRenderer()
{
	m_pRenderer = shared_ptr<Renderer>(new Renderer(m_pDeviceResources));
	return m_pRenderer->Initialize();
}

void D3DSystem::Render()
{
	m_pRenderer->Render();
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

//HWND D3DSystem::Create3DWindow(
//	INT32 screenWidth,
//	INT32 screenHeight,
//	BOOL vsync,
//	HWND* hwnd,
//	BOOL fullscreen)
//{
//	WNDCLASSEX wc;
//	DEVMODE dmScreenSettings;
//	int posX, posY;
//
//
//	// Get an external pointer to this object.
//	ApplicationHandle = this;
//
//	// Get the instance of this application.
//	auto applicatioInstance = GetModuleHandle(NULL);
//
//	// Give the application a name.
//	auto applicationName = L"Simple Rendering Engine";
//
//	// Setup the windows class with default settings.
//	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = applicatioInstance;
//	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//	wc.hIconSm = wc.hIcon;
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = applicationName;
//	wc.cbSize = sizeof(WNDCLASSEX);
//
//	// Register the window class.
//	RegisterClassEx(&wc);
//
//	// Determine the resolution of the clients desktop screen.
//	screenWidth = GetSystemMetrics(SM_CXSCREEN);
//	screenHeight = GetSystemMetrics(SM_CYSCREEN);
//
//	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
//	if (fullscreen)
//	{
//		// If full screen set the screen to maximum size of the users desktop and 32bit.
//		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
//		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
//		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
//		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
//		dmScreenSettings.dmBitsPerPel = 32;
//		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
//
//		// Change the display settings to full screen.
//		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
//		posX = posY = 0;
//	}
//	else
//	{
//		screenWidth = 800;
//		screenHeight = 600;
//
//		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
//		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
//	}
//
//	RECT rect = { 0, 0, screenWidth, screenHeight };
//	https://msdn.microsoft.com/en-us/library/windows/desktop/ms632665%28v=vs.85%29.aspx
//	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
//
//	auto handle =
//		CreateWindowEx(
//		WS_EX_APPWINDOW,
//		applicationName,
//		applicationName,
//		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
//		posX,
//		posY,
//		rect.right - rect.left,
//		rect.bottom - rect.top,
//		NULL,
//		NULL,
//		applicatioInstance,
//		NULL);
//
//	ShowWindow(handle, SW_SHOW);
//	SetForegroundWindow(handle);
//	SetFocus(handle);
//

//
//	return handle;
//}