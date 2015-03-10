#include "stdafx.h"
#include "d3dsystem.h"

D3DSystem::~D3DSystem()
{
	_renderer.reset();
	delete _deviceResources;
}

HRESULT D3DSystem::InitializeWithWindow(
	INT screenWidth,
	INT screenHeight,
	BOOL vsync,
	BOOL fullscreen)
{
	auto result = InitializeWindow(screenWidth, screenHeight);
	if (FAILED(result))
	{
		return result;
	}
	return InitializeForWindow(vsync, _windowInstance, _windowHandle, fullscreen);
}

HRESULT D3DSystem::InitializeWindow(INT screenWidth, INT screenHeight)
{
	HINSTANCE instance = 0;
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
		instance,
		NULL);

	if (!handle)
	{
		return false;
	}
	_windowInstance = &instance;
	_windowHandle = &handle;
	return true;
}

HRESULT D3DSystem::InitializeForWindow(
	BOOL vsync,
	HINSTANCE* windowInstance,
	HWND* windowHandle,
	BOOL fullscreen)
{
	_windowInstance = windowInstance;
	_windowHandle = windowHandle;
	_vSync = vsync;
	_fullScreen = fullscreen;

	return Initialize();
}

HRESULT D3DSystem::Initialize()
{
	POINT p;
	GetCursorPos(&p);
	_lastPointerPosition = new POINT{ 0.0, 0.0 };
	_trackInput = false;

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

	auto sc = rxsc::make_new_thread();
	auto so = rx::synchronize_in_one_worker(sc);
	rx::observable<>::interval(sc.now(), FPS(25), so)
		.subscribe([this](INT val)
	{
		D3DSystem::Render();
	}
	);
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
		NULL,
		D3D_DRIVER_TYPE_HARDWARE, // D3D_DRIVER_TYPE_WARP, //
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&device,
		&_d3dFeatureLevel,
		&context);

	if (FAILED(createResult))
	{
		createResult = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_SOFTWARE,
			0,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&device,
			&_d3dFeatureLevel,
			&context);
	}


	createResult = GetRenderQualitySettings(device);
	if (FAILED(createResult))
	{
		return createResult;
	}

	auto resources = new DeviceResources(device, context);

	auto viewport = CreateViewPort(_windowHandle);

	resources->ViewPort = new D3D11_VIEWPORT(*viewport);
	resources->Device = device;
	resources->DeviceContext = context;
	resources->WindowHandle = _windowHandle;
	resources->WindowInstance = _windowInstance;
	resources->FullScreen = _fullScreen;
	resources->VSync = _vSync;
	resources->NearZ = 0.0f;
	resources->FarZ = 1000.0f;
 
	//IOC::Register<DeviceResources>(resources);
	_deviceResources = resources;

	return createResult;
}

D3D11_VIEWPORT* D3DSystem::CreateViewPort(HWND *windowHandle)
{
	RECT windowRect;

	GetWindowRect(*windowHandle, &windowRect);
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto width = windowRect.right - windowRect.left;
	auto height = windowRect.bottom - windowRect.top;

	D3D11_VIEWPORT viewPort;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = D3D11_MIN_DEPTH;
	viewPort.MaxDepth = D3D11_MAX_DEPTH;

	return new D3D11_VIEWPORT(viewPort);
}

HRESULT D3DSystem::GetRenderQualitySettings(ID3D11Device *device)
{
	UINT numberOfLevels = 0;
	HRESULT result;
	vector<RenderingQuality> availableLevels;

	UINT maxSamples = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;

	for (UINT i = maxSamples; i > 0; i--)
	{
		result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &numberOfLevels);
		if (SUCCEEDED(result))
		{
			if (numberOfLevels < 1)
			{
				continue;
			}
			RenderingQuality quality = { i, numberOfLevels - 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
			availableLevels.push_back(quality);
		}
	}

	for (UINT i = maxSamples; i > 0; i--)
	{
		result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_UNORM, i, &numberOfLevels);
		if (SUCCEEDED(result))
		{
			if (numberOfLevels < 1)
			{
				continue;
			}
			RenderingQuality quality = { i, numberOfLevels - 1, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_D32_FLOAT, true };
			availableLevels.push_back(quality);
		}
	}

	for (UINT i = maxSamples; i > 0; i--)
	{
		result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &numberOfLevels);
		if (SUCCEEDED(result))
		{
			if (numberOfLevels < 1)
			{
				continue;
			}
			RenderingQuality quality = { i, numberOfLevels - 1, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_D32_FLOAT, true };
			availableLevels.push_back(quality);
		}
	}
	return result;
}

vector<MSAA>* D3DSystem::ProduceMsaaCapability(vector<MSAA> *options, INT i)
{
	auto localOptions = *options;
	auto masaa = MSAA_0X;
	switch (i)
	{
	case 1:
		masaa = MSAA_1X;
	case 2:
		masaa = MSAA_2X;
	case 4:
		masaa = MSAA_4X;
	case 8:
		masaa = MSAA_8X;
	}
	BOOL contains = false;
	for (int i = 0; localOptions.size(); i++)
	{
		if (localOptions[i] == masaa)
		{
			contains = true;
		}
	}
	if (!contains)
	{
		localOptions.push_back(masaa);
	}
	return new vector<MSAA>(localOptions);
}

HRESULT D3DSystem::CreateRenderer()
{
	auto scene = Scene::CreateStandardScene(_deviceResources);

	_renderer = shared_ptr<Renderer>(new Renderer(_deviceResources, true));
	return _renderer->Initialize(scene);
}

VOID D3DSystem::Render()
{
	_renderer->Render();
}

LRESULT D3DSystem::MessageHandler(HWND *hWnd, UINT message, WPARAM wparam, LPARAM lParam)
{
	if (this == NULL)
	{
		return 0;
	}
	if (lParam == 0)
	{
		return 0;
	}

	switch (message)
	{
	case WM_ACTIVATE:
	{
		return 0;
	}
	case WM_SIZE:
	{
		//auto viewPort = CreateViewPort(hWnd);
		//m_pRenderer->Resize(viewPort);
		return 0;
	}

	// press pointer (left mouse button)
	case WM_LBUTTONDOWN:
	{
		SetCapture(*_windowHandle);
		//ShowCursor(false);
		POINT p;
		auto result = GetCursorPos(&p);
		if (FAILED(result))
		{
			return result;
		}
		_lastPointerPosition = new POINT(p);
		_trackInput = true;
		return 0;
	}

	// release pointer (left mouse button)
	case WM_LBUTTONUP:
	{
		ReleaseCapture();
		_trackInput = false;
	}

	// scrolling (mouse wheel)
	case WM_POINTERWHEEL:
	{

	}
	case WM_POINTERHWHEEL:
	{

	}
	case WM_KEYDOWN:
	{

	}
	// move pointer (move mouse)
	case WM_MOUSEMOVE:
	{
		if (_trackInput)
		{
			POINT p;
			auto result = GetCursorPos(&p);
			if (FAILED(result))
			{
				return result;
			}
			auto offsetH = p.x - _lastPointerPosition->x;
			auto offsetV = p.y - _lastPointerPosition->y;
			auto offset = new XMFLOAT3(offsetH, 0.0f, offsetV);
			_renderer->GetScene()->GetCamera()->Rotate(offset);
			_lastPointerPosition = new POINT(p);
		}
		return 0;
	}
	}
	return 0;
}
