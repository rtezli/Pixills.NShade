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
}

HRESULT Renderer::Initialize()
{
    auto result = CreateDepthBuffer();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateDepthStencil();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateSwapChain();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateRenderTarget();
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


HRESULT Renderer::CreateRenderTargetDesciption()
{
    _renderTargetDesc.Width = Res::Get()->ViewPort->Width;
    _renderTargetDesc.Height = Res::Get()->ViewPort->Height;
    _renderTargetDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    _renderTargetDesc.ArraySize = 1;
    _renderTargetDesc.Format = Res::Get()->RenderQuality->TextureFormat;
    _renderTargetDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _renderTargetDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _renderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
    _renderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
    _renderTargetDesc.CPUAccessFlags = 0;
    _renderTargetDesc.MiscFlags = 0;

    return Res::Get()->Device->CreateTexture2D(&_renderTargetDesc, NULL, &_renderTarget);
}

HRESULT Renderer::CreateRenderTargetViewDesciption()
{
    _renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    _renderTargetViewDesc.Format = _renderTargetDesc.Format;
    return 0;
}

HRESULT Renderer::CreateRenderTarget()
{
    auto result = CreateRenderTargetDesciption();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateRenderTargetViewDesciption();
    if (FAILED(result))
    {
        return result;
    }

    if (_renderDeferred)
    {
        result = Res::Get()->Device->CreateRenderTargetView(_deferredBuffer, &_renderTargetViewDesc, &_renderTargetView);
    }
    else
    {
        result = Res::Get()->Device->CreateRenderTargetView(_backBuffer, &_renderTargetViewDesc, &_renderTargetView);
    }
    return result;
}


HRESULT Renderer::CreateDepthBufferDescription()
{
    _depthBufferDesc.Width = Res::Get()->ViewPort->Width;
    _depthBufferDesc.Height = Res::Get()->ViewPort->Height;
    _depthBufferDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    _depthBufferDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    _depthBufferDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _depthBufferDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    _depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    _depthBufferDesc.CPUAccessFlags = 0;
    _depthBufferDesc.MiscFlags = 0;
    _depthBufferDesc.ArraySize = 1;

    return 0;
}

HRESULT Renderer::CreateDepthBuffer()
{
    auto result = CreateDepthBufferDescription();
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateTexture2D(&_depthBufferDesc, NULL, &_depthStencilBuffer);
}

HRESULT Renderer::CreateDepthStencilDescription()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    depthStencilDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    depthStencilDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    depthStencilDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    depthStencilDesc.Width = Res::Get()->ViewPort->Width;
    depthStencilDesc.Height = Res::Get()->ViewPort->Height;

    _depthStencilDesc = depthStencilDesc;
    return 0;
}

HRESULT Renderer::CreateDepthStencilStateDescription()
{
    _depthStencilStateDesc.DepthEnable = true;
    _depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    _depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    _depthStencilStateDesc.StencilEnable = true;
    _depthStencilStateDesc.StencilReadMask = 0xFF;
    _depthStencilStateDesc.StencilWriteMask = 0xFF;

    _depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    _depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    _depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    _depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    auto result = Res::Get()->Device->CreateDepthStencilState(&_depthStencilStateDesc, &_depthStencilState);

    Res::Get()->DeviceContext->OMSetDepthStencilState(_depthStencilState, 1);
    return result;
}

HRESULT Renderer::CreateDepthStencilViewDescription()
{
    _depthStencilViewDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    _depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    _depthStencilViewDesc.Texture2D.MipSlice = 0;
    _depthStencilViewDesc.Flags = 0;

    return 0;
}

HRESULT Renderer::CreateDepthStencil()
{
    auto result = CreateDepthStencilDescription();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateDepthStencilViewDescription();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateDepthStencilStateDescription();
    if (FAILED(result))
    {
        return result;
    }

    result = Res::Get()->Device->CreateTexture2D(&_depthStencilDesc, NULL, &_depthStencil);
    result = Res::Get()->Device->CreateDepthStencilView(_depthStencil, &_depthStencilViewDesc, &_depthStencilView);

    if (FAILED(result))
    {
        return result;
    }
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

    if (_renderDeferred)
    {
        result = _swapChain->GetBuffer(0, IID_PPV_ARGS(&_deferredBuffer));
    }
    else
    {
        result = _swapChain->GetBuffer(0, IID_PPV_ARGS(&_backBuffer));
    }
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
    Res::Get()->DeviceContext->RSSetViewports(1, Res::Get()->ViewPort);
    return 0;
}


void Renderer::ClearScene()
{
    Res::Get()->DeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    Res::Get()->DeviceContext->ClearRenderTargetView(_renderTargetView, Res::Get()->DefaultColor);
    Res::Get()->DeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::Render(Scene *scene)
{
    ClearScene();
    //Res::Get()->DeviceContext->UpdateSubresource(Resources()->ConstBuffer, 0, nullptr, Resources()->ConstBufferData, 0, 0);
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
            auto buffers = vertexShader->GetBuffers();
            for (unsigned int b = 0; b < buffers.size() ; b++)
            {
                auto buffer = buffers.at(b);
                Res::Get()->DeviceContext->VSSetConstantBuffers(b, 1, &buffer);
            }

            auto resources = vertexShader->GetResources();
            for (unsigned int r = 0; r < resources.size(); r++)
            {
                auto resource = resources.at(r);
                Res::Get()->DeviceContext->VSSetShaderResources(r, 1, &resource);
            }
            Res::Get()->DeviceContext->VSSetShader(shaders->VertexShader->GetShader(), NULL, 0);
        }

        auto pixelshader = shaders->PixelShader;
        if (pixelshader)
        {
            auto buffers = pixelshader->GetBuffers();
            for (unsigned int b = 0; b < buffers.size(); b++)
            {
                auto buffer = buffers.at(b);
                Res::Get()->DeviceContext->PSSetConstantBuffers(b, 1, &buffer);
            }

            auto resources = pixelshader->GetResources();
            for (unsigned int r = 0; r < resources.size(); r++)
            {
                auto resource = resources.at(r);
                Res::Get()->DeviceContext->PSSetShaderResources(r, 1, &resource);
            }

            auto samplerState = pixelshader->GetSamplerState();
            if (samplerState)
            {
                Res::Get()->DeviceContext->PSSetSamplers(0, 1, &samplerState);
            }

            Res::Get()->DeviceContext->PSSetShader(shaders->PixelShader->GetShader(), NULL, 0);
        }

        Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }

    PostProcess();

    CopyToBackbuffer();

    _swapChain->Present(1, 0);
}

//void Renderer::SetupVertexShaders()
//{
//
//}

//void Renderer::SetupPixelShaders()
//{
//
//}

void Renderer::Tesselate(Shaders *shaders)
{
    if (!shaders->HullShader || !shaders->DomainShader)
    {
        return;
    }

}

void Renderer::PostProcess()
{

}

void Renderer::CopyToBackbuffer()
{

}


HRESULT	Renderer::ResizeSwapChain(UINT32 newWidth, UINT32 newHeight)
{
    return _swapChain->ResizeBuffers(_bufferCount, 0, 0, Res::Get()->RenderQuality->BufferFormat, _swapChainFlags);
}

HRESULT Renderer::Resize(D3D11_VIEWPORT* viewPort)
{
    HRESULT result;
    Res::Get()->ViewPort = viewPort;
    if (NULL != _swapChain && _isInitialized)
    {
        result = Initialize();
    }
    return result;
}