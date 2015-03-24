#pragma once

#include "res.h"
#include "d3dhelpers.h"

class ImmediateBuffer
{
public:
    ImmediateBuffer();
    void SetRenderTargets();
    void ClearRenderTargets();
private:
    ID3D11Texture2D*            _renderTargetTexture;
    ID3D11RenderTargetView*     _renderTargetView;
    ID3D11ShaderResourceView*   _shaderResourceView;
    ID3D11Texture2D*            _depthStencilBuffer;
    ID3D11DepthStencilView*     _depthStencilView;
};

