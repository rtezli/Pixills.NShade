#pragma once

#include "res.h"
#include "d3dhelpers.h"
#include "irendertarget.h"

class ImmediateTarget : public IRenderTarget
{
public:
    static ImmediateTarget*     Create();

    void Render();
    void SetRenderTargets();
    void ClearRenderTargets();

    ID3D11Texture2D*            const GetDepthStencilBuffer(){ return _depthStencilBuffer; };
    ID3D11RenderTargetView*     const GetRenderTargetView(){ return _renderTargetView; };
    ID3D11DepthStencilView*     const GetDepthStencilView(){ return _depthStencilView; };
    ID3D11DepthStencilState*    const GetDepthStencilState(){ return _depthStencilState; };

    void                        const SetDepthStencilBuffer(ID3D11Texture2D *texture){ _renderTargetTexture = texture; };
    void                        const SetRenderTargetView(ID3D11RenderTargetView *renderTargetView){ _renderTargetView = renderTargetView; };
    void                        const SetDepthStencilView(ID3D11DepthStencilView *depthStencilView){ _depthStencilView = depthStencilView; };
    void                        const SetDepthStencilState(ID3D11DepthStencilState *depthStencilState){ _depthStencilState = depthStencilState; };
private:
    ImmediateTarget();

    void CreateDepthStencilViewDescription();
    void CreateDepthStencilStateDescription();
    void CreateDepthStencil();

    ID3D11Texture2D             *_renderTargetTexture;
    ID3D11RenderTargetView      *_renderTargetView;

    ID3D11Texture2D             *_depthStencilBuffer;
    ID3D11DepthStencilView      *_depthStencilView;
    ID3D11DepthStencilState     *_depthStencilState;
};

