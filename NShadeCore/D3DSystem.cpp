#include "stdafx.h"
#include "d3dsystem.h"

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
	_windowInstance = &hInstance;
	_windowHandle = &handle;
	return true;
}

HRESULT D3DSystem::InitializeForWindow(
	BOOL vsync,
	HINSTANCE* hInstance,
	HWND* hwnd,
	BOOL fullscreen)
{
	_windowInstance = hInstance;
	_windowHandle = hwnd;
	_vSync = vsync;
	_fullScreen = fullscreen;

	//ShowCursor(false);

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

	Res::Get()->BufferCount = 2;
	Res::Get()->SwapChainFlags = 0;
	Res::Get()->DefaultColor = new FLOAT[4]{1.0f, 1.0f, 1.0f, 1.0f};
	// TODO : Calculate DPI
	Res::Get()->Dpi = 96.00;

	// TODO : Get available formats dynamically
	// 8 bit per channel (rgba)
	RenderingQuality quality0_8 = { 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, false }; // Should work for everyone
	RenderingQuality quality2_8 = { 2, 2, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality4_8 = { 16, 4, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality8_8 = { 32, 8, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true }; // Works on my machine ;)

	// 16 bit per channel  (rgba)
	RenderingQuality quality0_16 = { 0, 1, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_D32_FLOAT, true };

	Res::Get()->RenderQuality = new RenderingQuality(quality4_8);

	auto viewport = CreateViewPort(_windowHandle);

	Res::Get()->ViewPort = new D3D11_VIEWPORT(*viewport);
	Res::Get()->Device = device;
	Res::Get()->DeviceContext = context;
	Res::Get()->WindowHandle = _windowHandle;
	Res::Get()->WindowInstance = _windowInstance;
	Res::Get()->FullScreen = _fullScreen;
	Res::Get()->VSync = _vSync;
	Res::Get()->NearZ = 0.0f;
	Res::Get()->FarZ = 1000.0f;

	return createResult;
}

HRESULT D3DSystem::CreateInput()
{
	auto device = shared_ptr<Input>(new Input());
	return device->Initialize();
}

D3D11_VIEWPORT* D3DSystem::CreateViewPort(HWND* hwnd)
{
	RECT rect;

	GetWindowRect(*hwnd, &rect);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	auto width = rect.right - rect.left;
	auto height = rect.bottom - rect.top;

	D3D11_VIEWPORT viewPort;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = D3D11_MIN_DEPTH;
	viewPort.MaxDepth = D3D11_MAX_DEPTH;

	return new D3D11_VIEWPORT(viewPort);
}

HRESULT D3DSystem::GetRenderQualitySettings(ID3D11Device* device)
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

vector<MSAA>* D3DSystem::ProduceMsaaCapability(vector<MSAA>* options, INT i)
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
	for (INT i = 0; localOptions.size(); i++)
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

HRESULT D3DSystem::CreateCamera()
{
	_camera = shared_ptr<Camera>(new Camera());
	_camera->Initialize();
	return 0;
}

HRESULT D3DSystem::LoadModels()
{
	_model = shared_ptr<Model>(new Model());
	auto result = _model->Initialize();
	return 0;
}

HRESULT D3DSystem::CreateRenderer()
{
	_renderer = shared_ptr<Renderer>(new Renderer(true));
	return _renderer->Initialize();
}

VOID D3DSystem::Render()
{
	_renderer->Render();
}

LRESULT D3DSystem::MessageHandler(HWND* hWnd, UINT umessage, WPARAM wparam, LPARAM lParam)
{
	if (this == NULL)
	{
		return 0;
	}
	if (lParam == 0)
	{
		return 0;
	}

	switch (umessage)
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
			auto offset = new POINT{ offsetH, offsetV };
			_camera->Rotate(offset);
			_lastPointerPosition = new POINT(p);
		}
		return 0;
	}
	}
	return 0;
}
