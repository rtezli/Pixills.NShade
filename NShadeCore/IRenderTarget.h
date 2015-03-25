#pragma once

#include "d3d11.h"

class IRenderTarget
{
public:
    virtual ~IRenderTarget(){};
    virtual void Render() = 0;
    virtual void SetRenderTargets() = 0;
    virtual void ClearRenderTargets() = 0;

    virtual ID3D11RenderTargetView*     const GetRenderTargetView() = 0;
    virtual ID3D11DepthStencilView*     const GetDepthStencilView() = 0;
    virtual ID3D11DepthStencilState*    const GetDepthStencilState() = 0;
};