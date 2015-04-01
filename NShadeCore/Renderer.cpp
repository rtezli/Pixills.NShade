#include "stdafx.h"
#include "renderer.h"


Renderer::Renderer(bool useSwapChain)
{
    _renderDeferred = false;
    _isInitialized = false;
    _useSwapChain = useSwapChain;
    _rasterizerUseMultiSampling = true;
    _bufferCount = 2;
    _swapChainFlags = 0;

    //_renderTarget = ImmediateTarget::Create(Res::Get()->RenderQuality);
    _renderTarget = DeferredTarget::Create(Res::Get()->RenderQuality);
}

HRESULT Renderer::Initialize()
{
    _isInitialized = false;

    auto result = CreateSwapChain();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateRasterizer();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateViewPort();
    if (FAILED(result))
    {
        return result;
    }

    _isInitialized = true;
    return result;
}

HRESULT Renderer::CreateSwapChainDesciption()
{
    _swapChainDescription = { 0 };

    _swapChainDescription.BufferCount = _bufferCount;
    _swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    _swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    _swapChainDescription.Flags = _swapChainFlags;

    _swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
    _swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    _swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    _swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    _swapChainDescription.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _swapChainDescription.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _swapChainDescription.BufferDesc.Format =  Res::Get()->RenderQuality->DephStencilTextureFormat;//DXGI_FORMAT_R8G8B8A8_UNORM;
    _swapChainDescription.BufferDesc.Width = Res::Get()->RenderQuality->Width;
    _swapChainDescription.BufferDesc.Height = Res::Get()->RenderQuality->Height;

    auto handle = *Res::Get()->WindowHandle;
    _swapChainDescription.OutputWindow = handle;

    if (Res::Get()->FullScreen)
    {
        _swapChainDescription.Windowed = false;
    }
    else
    {
        _swapChainDescription.Windowed = true;
    }
    return 0;
}

HRESULT Renderer::CreateSwapChain()
{
    auto result = CreateSwapChainDesciption();
    if (FAILED(result))
    {
        return result;
    }

    IDXGIDevice *dxgiDevice = 0;
    IDXGIAdapter *dxgiAdapter = 0;
    IDXGIFactory *dxgiFactory = 0;

    result = Res::Get()->Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
    if (FAILED(result))
    {
        return result;
    }

    result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter);
    if (FAILED(result))
    {
        return result;
    }

    result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
    if (FAILED(result))
    {
        return result;
    }

    result = dxgiFactory->CreateSwapChain(Res::Get()->Device, &_swapChainDescription, &_swapChain);
    if (FAILED(result))
    {
        return result;
    }

    result = _swapChain->GetBuffer(0, IID_PPV_ARGS(&_backBuffer));
    if (FAILED(result))
    {
        return result;
    }

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();

    //_renderTarget->CreateRenderTarget(_backBuffer);
    return 0;
}


HRESULT Renderer::CreateRasterizerDescription()
{
    auto isRightHand = false;

    _rasterizerDesc.AntialiasedLineEnable = _rasterizerUseMultiSampling;
    _rasterizerDesc.DepthBias = 0;
    _rasterizerDesc.DepthBiasClamp = 0.0f;
    _rasterizerDesc.DepthClipEnable = true;
    _rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    //_rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

    _rasterizerDesc.MultisampleEnable = _rasterizerUseMultiSampling;
    _rasterizerDesc.ScissorEnable = false;
    _rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    _rasterizerDesc.CullMode = D3D11_CULL_NONE;// D3D11_CULL_BACK;//
    _rasterizerDesc.FrontCounterClockwise = true;

    return 0;
}

HRESULT Renderer::CreateRasterizer()
{
    auto result = CreateRasterizerDescription();
    if (FAILED(result))
    {
        return result;
    }

    result = Res::Get()->Device->CreateRasterizerState(&_rasterizerDesc, &Res::Get()->RasterizerState);
    if (FAILED(result))
    {
        return result;
    }

    Res::Get()->DeviceContext->RSSetState(Res::Get()->RasterizerState);

    return 0;
}

HRESULT Renderer::CreateViewPort()
{
    auto viewPort = Res::Get()->ViewPort;
    Res::Get()->DeviceContext->RSSetViewports(1, viewPort);
    return 0;
}


void Renderer::ClearScene()
{
    _renderTarget->ClearRenderTargets();
}

void Renderer::Render(Scene *scene)
{
    if (!_isInitialized)
    {
        return;
    }

    ClearScene();

    for (unsigned int m = 0; m < scene->GetModels()->size(); m++)
    {
        auto model = scene->GetModels()->at(m);
        auto material = model.GetMaterial();
        auto shaders = material->GetShaders();

        unsigned int offset = 0;

        auto vertexBuffer = model.GetVertexBuffer();
        auto strides = shaders->VertexShader->GetInputSize();
        auto indexBuffer = model.GetIndexBuffer();

        Res::Get()->DeviceContext->IASetInputLayout(shaders->VertexShader->GetInputLayout());
        Res::Get()->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
        Res::Get()->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        Res::Get()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        auto vertexShader = shaders->VertexShader;
        if (vertexShader)
        {
            vertexShader->Render();
        }

        auto hullShader = shaders->HullShader;
        if (hullShader)
        {
            hullShader->Render();
        }

        auto domainShader = shaders->DomainShader;
        if (domainShader)
        {
            domainShader->Render();
        }

        auto geometryShader = shaders->GeometryShader;
        if (geometryShader)
        {
            geometryShader->Render();
        }

        auto pixelshader = shaders->PixelShader;
        if (pixelshader)
        {
            pixelshader->Render();
        }

        Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }

    auto steps = scene->GetPostProcessingSteps();
    if (steps)
    {
        // Unbind render target
        //_renderTarget->SetInput();
        for (unsigned int p = 0; p < steps->size(); p++)
        {
            auto step = steps->at(p);
            auto input = _renderTarget->GetRenderTargetAsResource();
            auto output = step.ApplyOn(input);
            Res::Get()->DeviceContext->DrawAuto();// DrawIndexed(model.GetIndexCount(), 0, 0);
        }
        // Rebind render target
        //_renderTarget->SetOutput();
        //Res::Get()->DeviceContext->DrawAuto();//DrawIndexed(model.GetIndexCount(), 0, 0);
    }

    _swapChain->Present(1, 0);
}

void Renderer::Tesselate(Shaders *shaders)
{
    if (!shaders->HullShader || !shaders->DomainShader)
    {
        return;
    }
}

void Renderer::Resize()
{
    Initialize();
}