#pragma once

const int BUFFER_COUNT = 2;

#include "res.h"
#include "d3dhelpers.h"
#include "irendertarget.h"

class DeferredTarget : public IRenderTarget
{
public:
    static DeferredTarget*     Create();

    void Render();
    void SetRenderTargets();
    void ClearRenderTargets();

    ID3D11Texture2D*            const GetDepthStencilBuffer(){ return nullptr; };
    ID3D11RenderTargetView*     const GetRenderTargetView(){ return nullptr; };
    ID3D11DepthStencilView*     const GetDepthStencilView(){ return _depthStencilView; };
    ID3D11DepthStencilState*    const GetDepthStencilState(){ return _depthStencilState; };
private:
    DeferredTarget();
    void CreateDepthStencilViewDescription();
    void CreateDepthStencilStateDescription();
    void CreateDepthStencil();

    ID3D11Texture2D             *_renderTargetTextureArray[BUFFER_COUNT];
    ID3D11RenderTargetView      *_renderTargetViewArray[BUFFER_COUNT];
    ID3D11ShaderResourceView    *_shaderResourceViewArray[BUFFER_COUNT];

    ID3D11Texture2D             *_depthStencilBuffer;
    ID3D11DepthStencilView      *_depthStencilView;
    ID3D11DepthStencilState     *_depthStencilState;
};