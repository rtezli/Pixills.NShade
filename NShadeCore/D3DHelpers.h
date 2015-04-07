#pragma once

#include "d3d11.h"
#include "res.h"
 
class D3DHelpers
{
public:
    static ID3D11Buffer*                CreateConstBuffer(char *data, unsigned int size);
    static ID3D11Texture2D*             CreateTexture(D3D11_BIND_FLAG bindFlags, RenderingQuality *quality);
    static ID3D11Texture2D*             CreateTexture(D3D11_BIND_FLAG bindFlags, RenderingQuality *quality, DXGI_FORMAT format);
    static ID3D11RenderTargetView*      CreateRenderTarget(ID3D11Resource *resource, D3D11_RTV_DIMENSION dimensions, RenderingQuality *quality);
    static ID3D11ShaderResourceView*    CreateRandom1DTexture(unsigned int size);
    static ID3D11ShaderResourceView*    CreateRandom2DTexture(unsigned int width, unsigned int height);
    static ID3D11DepthStencilView*      CreateDepthStencilView();
    static float                        RandomFloat(float min, float max);
    static XMFLOAT4*                    RandomXMFLOAT4();
};