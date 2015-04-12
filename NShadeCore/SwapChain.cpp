#include "stdafx.h"
#include "swapchain.h"


SwapChain::SwapChain()
{

}


SwapChain::~SwapChain()
{

}

HRESULT SwapChain::CreateSwapChainDesciption()
{
    _swapChainDescription = { 0 };

    _swapChainDescription->BufferCount = _bufferCount;
    _swapChainDescription->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    _swapChainDescription->SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    _swapChainDescription->Flags = _swapChainFlags;

    _swapChainDescription->BufferDesc.RefreshRate.Numerator = 0;
    _swapChainDescription->BufferDesc.RefreshRate.Denominator = 1;
    _swapChainDescription->BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    _swapChainDescription->BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    _swapChainDescription->SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _swapChainDescription->SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _swapChainDescription->BufferDesc.Format = Res::Get()->RenderQuality->DephStencilTextureFormat;
    _swapChainDescription->BufferDesc.Width = Res::Get()->RenderQuality->Width;
    _swapChainDescription->BufferDesc.Height = Res::Get()->RenderQuality->Height;

    auto handle = *Res::Get()->WindowHandle;
    _swapChainDescription->OutputWindow = handle;

    if (Res::Get()->FullScreen)
    {
        _swapChainDescription->Windowed = false;
    }
    else
    {
        _swapChainDescription->Windowed = true;
    }
    return 0;
}

HRESULT SwapChain::CreateSwapChain()
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

    result = dxgiFactory->CreateSwapChain(Res::Get()->Device, _swapChainDescription, &_swapChain);
    if (FAILED(result))
    {
        return result;
    }

    result = _swapChain->GetBuffer(0, IID_PPV_ARGS(&_backBufferTexture));
    if (FAILED(result))
    {
        return result;
    }

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();

    auto quality = new RenderingQuality();
    quality->TextureFormat = _swapChainDescription->BufferDesc.Format;

    _backBufferTarget = D3DHelpers::CreateRenderTarget(_backBufferTexture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);
    return 0;
}

HRESULT SwapChain::Initialize()
{
    auto result = CreateSwapChainDesciption();
    if (FAILED(result))
    {
        return result;
    }
    return CreateSwapChain();
}