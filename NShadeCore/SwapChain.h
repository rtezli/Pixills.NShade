#pragma once

#include "d3d11.h"
#include "d3dhelpers.h"
#include "irendertarget.h"
#include "res.h"

class SwapChain
{
public:
    SwapChain();
    ~SwapChain();
    HRESULT Initialize();
private:
    HRESULT                 CreateSwapChainDesciption();
    HRESULT                 CreateSwapChain();
private:
    DXGI_SWAP_CHAIN_DESC    *_swapChainDescription;
    ID3D11RenderTargetView  *_renderTargetView;
    IRenderTarget           *_renderTarget;
    ID3D11Texture2D         *_backBufferTexture;
    ID3D11RenderTargetView  *_backBufferTarget;
    IDXGISwapChain          *_swapChain;
    int                     _swapChainFlags;
    int                     _bufferCount;
};

