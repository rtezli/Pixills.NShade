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
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	m_viewportWidth = screenWidth;
	m_viewportHeight = screenHeight;
	m_nearZ = screenNear;
	m_farZ = screenDepth;

	auto handle = InitializeWindow(screenWidth, screenHeight);
	InitializeForWindow(vsync, handle, fullscreen, screenDepth, screenNear);
	return 0;
}

HRESULT D3DSystem::InitializeForWindow(
	bool vsync,
	std::shared_ptr<HWND> hwnd,
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	m_pWindow = std::shared_ptr<Window>(new Window(hwnd, vsync, fullscreen));
	Initialize();
	return 0;
}

std::shared_ptr<HWND> D3DSystem::InitializeWindow(int screenWidth, int screenHeight)
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
	return std::shared_ptr<HWND>(&handle);
}

HRESULT D3DSystem::Initialize()
{
	CreateDevice();
	CreateCamera();
	LoadModels();
	CreateRenderer();
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
	m_pDeviceResources = new DeviceResources(device, context);
	return createResult;
}

HRESULT D3DSystem::SetCamera(XMVECTOR position, XMVECTOR direction, unsigned short viewAngle)
{
	return 0;
}

HRESULT D3DSystem::SetCamera(XMVECTOR position, XMVECTOR direction, float focalLength)
{
	return 0;
}

HRESULT D3DSystem::CreateCamera()
{
	m_pCamera = std::shared_ptr<Camera>(new Camera(m_viewportWidth, m_viewportHeight, m_nearZ, m_farZ));
	return 0;
}

HRESULT D3DSystem::LoadModels()
{
	m_pModel = std::shared_ptr<Model>(new Model());
	auto cube = m_pModel->Cube;
	auto size = cube.size();
	auto result = m_pModel->Initialize(m_pDeviceResources->Device, &cube, size);
	return 0;
}

HRESULT D3DSystem::CreateRenderer()
{
	m_pRenderer = std::shared_ptr<Renderer>(new Renderer(m_pDeviceResources, m_pWindow));
	return 0;
}

void D3DSystem::Render()
{

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
//	// Hide the mouse cursor.
//	// ShowCursor(false);
//
//	return handle;
//}