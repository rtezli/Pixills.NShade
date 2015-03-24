#pragma once

const int BUFFER_COUNT = 2;

#include "res.h"
#include "d3dhelpers.h"

class DeferredBuffer
{
public:
    DeferredBuffer();
    ID3D11Texture2D*            _renderTargetTextureArray[BUFFER_COUNT];
    ID3D11RenderTargetView*     _renderTargetViewArray[BUFFER_COUNT];
    ID3D11ShaderResourceView*   _shaderResourceViewArray[BUFFER_COUNT];
    ID3D11Texture2D*            _depthStencilBuffer;
    ID3D11DepthStencilView*     _depthStencilView;
};