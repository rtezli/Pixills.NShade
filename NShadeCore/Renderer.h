#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#ifndef PS_PROFILE
#define PS_PROFILE {"ps_5_0" }
#endif

#ifndef VS_PROFILE
#define VS_PROFILE {"vs_5_0" }
#endif

#ifndef GS_PROFILE
#define GS_PROFILE { "ps_5_0" }
#endif

#ifndef HS_PROFILE
#define HS_PROFILE { "hs_5_0" }
#endif

#ifndef DS_PROFILE
#define DS_PROFILE { "ds_5_0" }
#endif

#include "d3d11.h"
#include "d3dcompiler.h"
#include "phongvertexshader.h"
#include "scene.h"

EXTERN class API Renderer
{
public:
    Renderer(bool useSwapChain);
public:
    HRESULT	ResizeSwapChain(UINT32 newWidth, UINT32 newHeight);
    HRESULT SetShaderParameters();
    HRESULT	Initialize();
    HRESULT	Resize(D3D11_VIEWPORT* viewport);
public:
    void ClearScene();
    void Render(Scene *scene);
    void PostProcess();
    void CopyToBackbuffer();
    bool const Initialized(){ return _isInitialized; };
private:
    /* render target */
    HRESULT CreateRenderTargetDesciption();
    HRESULT CreateRenderTargetViewDesciption();
    HRESULT CreateRenderTarget();

    /* swap chain */
    HRESULT	CreateSwapChainDesciption();
    HRESULT CreateSwapChain();

    /* constant buffer */
    HRESULT CreateConstantBufferDescription();
    HRESULT CreateConstantBuffer();

    /* depth buffer */
    HRESULT CreateDepthBufferDescription();
    HRESULT CreateDepthBuffer();

    /* depth stencil */
    HRESULT CreateDepthStencilDescription();
    HRESULT CreateDepthStencilViewDescription();
    HRESULT CreateDepthStencilStateDescription();
    HRESULT CreateDepthStencil();

    /* rasterizer */
    HRESULT CreateRasterizerDescription();
    HRESULT CreateRasterizer();

    HRESULT CreateViewPort();
private:
    ID3D11Texture2D                 *_backBuffer;
    ID3D11Texture2D                 *_deferredBuffer;
    ID3D11Texture2D                 *_depthStencilBuffer;

    ID3D11Texture2D                 *_renderTarget;
    ID3D11DepthStencilState         *_depthStencilState;
    ID3D11DepthStencilView          *_depthStencilView;
    ID3D11RenderTargetView          *_renderTargetView;

    IDXGISwapChain                  *_swapChain;
    int                             _swapChainFlags;
    int                             _bufferCount;

    D3D11_TEXTURE2D_DESC            _depthBufferDesc;
    DXGI_SWAP_CHAIN_DESC            _swapChainDescription;

    D3D11_TEXTURE2D_DESC            _depthStencilDesc;
    D3D11_DEPTH_STENCIL_DESC        _depthStencilStateDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC   _depthStencilViewDesc;

    D3D11_TEXTURE2D_DESC            _renderTargetDesc;
    D3D11_RENDER_TARGET_VIEW_DESC   _renderTargetViewDesc;

    D3D11_RASTERIZER_DESC           _rasterizerDesc;

    bool                            _isInitialized;
    bool                            _useSwapChain;
    bool                            _renderDeferred;
    bool                            _rasterizerUseMultiSampling;
};