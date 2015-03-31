#pragma once

const int BUFFER_COUNT = 2;

#include "res.h"
#include "d3dhelpers.h"
#include "irendertarget.h"

class DeferredTarget : public IRenderTarget
{
public:
    static DeferredTarget*     Create(RenderingQuality *quality);

    void Render();
    void SetRenderTargets();
    void ClearRenderTargets();
    void CreateRenderTarget(ID3D11Resource *buffer);
    void SetInput();
    void SetOutput();

    ID3D11Texture2D*            const GetDepthStencilBuffer(){ return _depthStencilBuffer; }
    ID3D11Texture2D*            const GetRenderTarget(){ return _renderTargetTexture; }
    ID3D11RenderTargetView*     const GetRenderTargetView(){ return _renderTargetView; }
    ID3D11ShaderResourceView*   const GetShaderResourceView(){ return _shaderResourceView; }

    ID3D11DepthStencilView*     const GetDepthStencilView(){ return _depthStencilView; }
    ID3D11DepthStencilState*    const GetDepthStencilState(){ return _depthStencilState; }

    void                        const SetDepthStencilBuffer(ID3D11Texture2D *texture){ _depthStencilBuffer = texture; }
    void                        const SetRenderTargetView(ID3D11RenderTargetView *renderTargetView){ _renderTargetView = renderTargetView; }
    void                        const SetShaderResourceView(ID3D11ShaderResourceView *resourceView){ _shaderResourceView = resourceView; }

    void                        const SetDepthStencilView(ID3D11DepthStencilView *depthStencilView){ _depthStencilView = depthStencilView; }
    void                        const SetDepthStencilState(ID3D11DepthStencilState *depthStencilState){ _depthStencilState = depthStencilState; }
private:
    DeferredTarget(RenderingQuality *quality);
    void CreateDepthStencilViewDescription();
    void CreateDepthStencilStateDescription();
    void CreateDepthStencil();

    RenderingQuality            *_quality;
    ID3D11Resource              *_finalTarget;
    ID3D11Texture2D             *_renderTargetTexture;
    ID3D11RenderTargetView      *_renderTargetView;
    ID3D11ShaderResourceView    *_shaderResourceView;

    ID3D11Texture2D             *_depthStencilBuffer;
    ID3D11DepthStencilView      *_depthStencilView;
    ID3D11DepthStencilState     *_depthStencilState;
};