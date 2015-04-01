#pragma once

#include "d3d11.h"

class IRenderTarget
{
public:
    virtual ~IRenderTarget(){};
    virtual void Render() = 0;
    virtual void SetRenderTargets() = 0;
    virtual void ClearRenderTargets() = 0;
    virtual void Prepare() = 0;

    virtual ID3D11Texture2D*            GetRenderTargetAsResource() = 0;

    virtual ID3D11Texture2D*            const GetDepthStencilBuffer() = 0;
    virtual ID3D11Texture2D*            const GetRenderTarget() = 0;
    virtual ID3D11RenderTargetView*     const GetRenderTargetView() = 0;
    virtual ID3D11ShaderResourceView*   const GetShaderResourceView() = 0;

    virtual ID3D11DepthStencilView*     const GetDepthStencilView() = 0;
    virtual ID3D11DepthStencilState*    const GetDepthStencilState() = 0;

    virtual void                        const SetDepthStencilBuffer(ID3D11Texture2D *texture) = 0;
    virtual void                        const SetRenderTargetView(ID3D11RenderTargetView *renderTargetView) = 0;
    virtual void                        const SetShaderResourceView(ID3D11ShaderResourceView *resourceView) = 0;

    virtual void                        const SetDepthStencilView(ID3D11DepthStencilView *depthStencilView) = 0;
    virtual void                        const SetDepthStencilState(ID3D11DepthStencilState *depthStencilState) = 0;
protected:
    RenderingQuality            *_quality;

    ID3D11Texture2D             *_renderTargetTexture;
    ID3D11RenderTargetView      *_renderTargetView;

    ID3D11Texture2D             *_backBufferTexture;
    ID3D11RenderTargetView      *_backBufferView;

    ID3D11ShaderResourceView    *_shaderResourceView;

    ID3D11Texture2D             *_depthStencilBuffer;
    ID3D11DepthStencilView      *_depthStencilView;
    ID3D11DepthStencilState     *_depthStencilState;
};