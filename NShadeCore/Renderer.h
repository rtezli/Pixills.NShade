#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "d3d11.h"
#include "d3dcompiler.h"
#include "d3dhelpers.h"
#include "scene.h"
#include "irendertarget.h"
#include "immediatetarget.h"
#include "deferredtarget.h"

EXTERN class API Renderer
{
public:
    Renderer(bool useSwapChain);
public:
    HRESULT ResizeSwapChain(UINT32 newWidth, UINT32 newHeight);
    HRESULT SetShaderParameters();
    HRESULT Initialize();
    void Resize();
public:
    void ClearScene();
    void Render(Scene *scene);
    bool const Initialized(){ return _isInitialized; };
private:
    void PostProcess(Scene *scene, IRenderTarget *target, unsigned int indexCount);
    void Tesselate(Shaders *shaders);

    /* render target */

    HRESULT CreateImmediateRenderTarget();
    //HRESULT CreateDeferredRenderTarget();

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
    ID3D11Texture2D                 *_backBufferTexture;
    ID3D11RenderTargetView          *_backBufferTarget;
    IRenderTarget                   *_renderTarget;

    IDXGISwapChain                  *_swapChain;
    DXGI_SWAP_CHAIN_DESC            _swapChainDescription;
    int                             _swapChainFlags;

    int                             _bufferCount;

    D3D11_RASTERIZER_DESC           _rasterizerDesc;

    bool                            _isInitialized;
    bool                            _useSwapChain;
    bool                            _renderDeferred;
    bool                            _rasterizerUseMultiSampling;
};