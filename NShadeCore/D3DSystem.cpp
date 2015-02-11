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

	//ShowCursor(false);

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

	D3D11_VIEWPORT viewPort;
	viewPort.Width = m_viewportWidth;
	viewPort.Height = m_viewportHeight;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = D3D11_MIN_DEPTH;
	viewPort.MaxDepth = D3D11_MAX_DEPTH;

	createResult = GetRenderQualitySettings(device);
	if (FAILED(createResult))
	{
		return createResult;
	}

	auto resources = new DeviceResources(device, context);

	resources->Device = device;
	resources->DeviceContext = context;
	resources->WindowHandle = m_pWindowHandle;
	resources->FullScreen = m_fullScreen;
	resources->VSync = m_vSync;
	resources->ViewPort = new D3D11_VIEWPORT(viewPort);
	resources->NearZ = 0.0f;
	resources->FarZ = 1000.0f;

	m_pDeviceResources = resources;

	return createResult;
}

HRESULT D3DSystem::GetRenderQualitySettings(ID3D11Device* device)
{
	UINT level = 0;
	int index = 0;
	HRESULT result;
	vector<RenderingQuality> availableLevels;

	RenderingQuality defaultQuality = { 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM, false };
	availableLevels.push_back(defaultQuality);

	for (UINT i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++)
	{
		result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &level);
		if (SUCCEEDED(result))
		{
			if (level < 1)
			{
				continue;
			}

			RenderingQuality quality = { level, i, DXGI_FORMAT_R8G8B8A8_UNORM, true };
			availableLevels.push_back(quality);
			index++;
		}
	}

	for (UINT i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++)
	{
		result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, i, &level);
		if (SUCCEEDED(result))
		{
			if (level < 1)
			{
				continue;
			}

			RenderingQuality quality = { level, i, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, true };
			availableLevels.push_back(quality);
			index++;
		}
	}

	index = 0;
	for (UINT i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++)
	{
		UINT level = 0;
		result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &level);
		if (SUCCEEDED(result))
		{
			if (level < 1)
			{
				continue;
			}

			RenderingQuality quality = { level, i, DXGI_FORMAT_D24_UNORM_S8_UINT, true };
			availableLevels.push_back(quality);
			index++;
		}
	}

	return result;
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
	m_pRenderer->Render();
}

LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
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