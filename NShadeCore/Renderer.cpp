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
    _immediateTarget = ImmediateTarget::Create();
    _deferredTarget = DeferredTarget::Create();

}

HRESULT Renderer::Initialize()
{
    _isInitialized = false;

    auto result = CreateDepthStencil();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateSwapChain();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateImmediateRenderTarget();
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


HRESULT Renderer::CreateImmediateRenderTarget()
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    renderTargetViewDesc.Format = Res::Get()->RenderQuality->TextureFormat;

    auto target = _immediateTarget->GetRenderTargetView();
    ID3D11RenderTargetView *targetView;
    Res::Get()->Device->CreateRenderTargetView(_backBuffer, &renderTargetViewDesc, &targetView);
    _immediateTarget->SetRenderTargetView(targetView);

    return 0;
}


HRESULT Renderer::CreateDepthStencilStateDescription()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilStateDesc.StencilEnable = true;
    depthStencilStateDesc.StencilReadMask = 0xFF;
    depthStencilStateDesc.StencilWriteMask = 0xFF;

    depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    auto depthStencilState = _immediateTarget->GetDepthStencilState();
    auto result = Res::Get()->Device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
    Res::Get()->DeviceContext->OMSetDepthStencilState(depthStencilState, 1);
    return result;
}

HRESULT Renderer::CreateDepthStencil()
{
    auto result = CreateDepthStencilStateDescription();
    if (FAILED(result))
    {
        return result;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    auto depthStencilBuffer = _immediateTarget->GetDepthStencilBuffer();
    auto depthStencilView = _immediateTarget->GetDepthStencilView();

    ID3D11DepthStencilView* view;
    Res::Get()->Device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &view);
    _immediateTarget->SetDepthStencilView(view);
    return 0;
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
    _swapChainDescription.BufferDesc.Format = Res::Get()->RenderQuality->TextureFormat;
    _swapChainDescription.BufferDesc.Width = Res::Get()->ViewPort->Width;
    _swapChainDescription.BufferDesc.Height = Res::Get()->ViewPort->Height;

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
    auto renderTarget = _immediateTarget->GetRenderTargetView();
    auto depthStencil = _immediateTarget->GetDepthStencilView();

    Res::Get()->DeviceContext->OMSetRenderTargets(1, &renderTarget, depthStencil);
    Res::Get()->DeviceContext->ClearRenderTargetView(renderTarget, Res::Get()->DefaultColor);
    Res::Get()->DeviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
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

        auto steps = scene->GetPostProcessingSteps();
        if (steps)
        {
            for (unsigned int p = 0; p < steps->size(); p++)
            {
                auto step = steps->at(p);
                //_backBuffer = step.ApplyOn(_backBuffer);
                Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
            }
        }
    }

    PostProcess(scene);

    CopyToBackbuffer();

    _swapChain->Present(1, 0);
}

void Renderer::Tesselate(Shaders *shaders)
{
    if (!shaders->HullShader || !shaders->DomainShader)
    {
        return;
    }
}

void Renderer::PostProcess(Scene *scene)
{

}

void Renderer::CopyToBackbuffer()
{

}

void Renderer::Resize()
{
    Initialize();
}