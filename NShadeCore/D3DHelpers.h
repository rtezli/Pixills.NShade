#pragma once

#include "d3d11.h"
#include "res.h"

class D3DHelpers
{
public:
    static ID3D11Buffer*            CreateBuffer(char *data, unsigned int size, D3D11_BIND_FLAG bindFlags);
    static ID3D11Texture2D*         CreateTexture(D3D11_BIND_FLAG bindFlags);
    static ID3D11RenderTargetView*  CreateRenderTarget(ID3D11Resource *resource, D3D11_RTV_DIMENSION dimensions);
};