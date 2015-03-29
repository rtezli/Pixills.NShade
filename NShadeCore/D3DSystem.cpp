#include "stdafx.h"
#include "d3dsystem.h"

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
    return InitializeForWindow(vsync, Res::Get()->WindowInstance, Res::Get()->WindowHandle, fullscreen);
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
    Res::Get()->WindowInstance = &hInstance;
    Res::Get()->WindowHandle = &handle;
    return true;
}

HRESULT D3DSystem::InitializeForWindow(bool vsync, HINSTANCE* windowInstance, HWND* windowHandle, bool fullscreen)
{
    Res::Get()->WindowInstance = windowInstance;
    Res::Get()->WindowHandle = windowHandle;
    Res::Get()->VSync = vsync;
    Res::Get()->FullScreen = fullscreen;
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

    CreateScene();

    auto sc = rxsc::make_new_thread();
    auto so = rx::synchronize_in_one_worker(sc);
    rx::observable<>::interval(sc.now(), FPS(25), so)
        .subscribe([this](int val)
    {
        D3DSystem::Render();
    }
    );
    return result;
}

HRESULT D3DSystem::CreateDevice()
{
    unsigned int creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    ID3D11DeviceContext* deferredContext;

 
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

    device->CreateDeferredContext(0, &deferredContext);

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

    Res::Get()->DefaultColor = new float[4]{0.0f, 0.0f, 0.0f, 0.0f};

    ID2D1Factory *factory;
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    factory->GetDesktopDpi(&Res::Get()->DpiX, &Res::Get()->DpiY);

    CreateViewPort();

    createResult = GetRenderQualitySettings(device);
    if (FAILED(createResult))
    {
        return createResult;
    }

    Res::Get()->Device = device;
    Res::Get()->DeviceContext = context;
    Res::Get()->DeferredContext = deferredContext;
    Res::Get()->NearZ = 0.0f;
    Res::Get()->FarZ = 1000.0f;

    return createResult;
}

HRESULT D3DSystem::CreateInput()
{
    auto device = shared_ptr<Input>(new Input());
    return device->Initialize();
}

void D3DSystem::CreateViewPort()
{
    RECT rect;
    auto handle = Res::Get()->WindowHandle;
    GetWindowRect(*handle, &rect);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    auto width = rect.right - abs(rect.left);
    auto height = rect.bottom - abs(rect.top);

    D3D11_VIEWPORT viewPort;
    viewPort.Width = width;
    viewPort.Height = height;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.MinDepth = D3D11_MIN_DEPTH;
    viewPort.MaxDepth = D3D11_MAX_DEPTH;

    Res::Get()->ViewPort = new D3D11_VIEWPORT(viewPort);
}

HRESULT D3DSystem::GetRenderQualitySettings(ID3D11Device* device)
{
    unsigned int numberOfLevels = 0;
    HRESULT result;
    vector<RenderingQuality> availableLevels;

    unsigned int maxSamples = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;

    for (unsigned int i = maxSamples; i > 0; i--)
    {
        result = device->CheckMultisampleQualityLevels(DXGI_FORMAT_R32G32B32A32_FLOAT, i, &numberOfLevels);
        if (SUCCEEDED(result))
        {
            if (numberOfLevels < 1)
            {
                continue;
            }
            auto width = Res::Get()->ViewPort->Width;
            auto height = Res::Get()->ViewPort->Height;
            RenderingQuality quality = { width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R8G8B8A8_UNORM, numberOfLevels - 1, i, true };
            availableLevels.push_back(quality);
        }
    }
    auto highestQuality = availableLevels.front();
    Res::Get()->RenderQuality = new RenderingQuality(highestQuality);
    return result;
}

vector<MSAA>* D3DSystem::ProduceMsaaCapability(vector<MSAA>* options, int i)
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
    bool contains = false;
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
    _renderer = shared_ptr<Renderer>(new Renderer(true));
    return _renderer->Initialize();
}

void D3DSystem::CreateScene()
{
    _scene = Scene::CreateStandardScene();
}

void D3DSystem::Render()
{
    _renderer->Render(_scene);
}

LRESULT D3DSystem::MessageHandler(HWND* hWnd, unsigned int umessage, WPARAM wparam, LPARAM lParam)
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
        CreateViewPort();
        _renderer->Resize();
        return 0;
    }

    // press pointer (left mouse button)
    case WM_LBUTTONDOWN:
    {
        SetCapture(*Res::Get()->WindowHandle);
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
            _scene->GetCamera()->Rotate(offset);
            _lastPointerPosition = new POINT(p);
        }
        return 0;
    }
    }
    return 0;
}
