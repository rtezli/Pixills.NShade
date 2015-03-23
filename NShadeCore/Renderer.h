#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "d3d11.h"
#include "d3dcompiler.h"
#include "d3dhelpers.h"
#include "scene.h"

EXTERN class API Renderer
{
public:
    Renderer(bool useSwapChain);
public:
    HRESULT	ResizeSwapChain(UINT32 newWidth, UINT32 newHeight);
    HRESULT SetShaderParameters();
    HRESULT	Initialize();
    void Resize();
public:
    void ClearScene();
    void Render(Scene *scene);
    bool const Initialized(){ return _isInitialized; };
private:
    void Tesselate(Shaders *shaders);
    void PostProcess(Scene *scene);
    void CopyToBackbuffer();
    
    /* render target */

    HRESULT CreateImmediateRenderTarget();
    HRESULT CreateDeferredRenderTarget();

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
    ID3D11Texture2D                 *_depthStencil;
    ID3D11Texture2D                 *_depthStencilBuffer;

    ID3D11Texture2D                 *_renderTarget;
    ID3D11RenderTargetView          *_renderTargetView;

    ID3D11DepthStencilState         *_depthStencilState;
    ID3D11DepthStencilView          *_depthStencilView;


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