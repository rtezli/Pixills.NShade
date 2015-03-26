#include "stdafx.h"
#include "d3dhelpers.h"

ID3D11Buffer* D3DHelpers::CreateBuffer(char *data, unsigned int size, D3D11_BIND_FLAG bindFlags)
{
    D3D11_BUFFER_DESC bufferDesc = { 0 };
    bufferDesc.ByteWidth = size;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = bindFlags;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA bufferData = { 0 };
    bufferData.pSysMem = data;
    bufferData.SysMemPitch = 0;
    bufferData.SysMemSlicePitch = 0;

    ID3D11Buffer* buffer;
    Res::Get()->Device->CreateBuffer(&bufferDesc, &bufferData, &buffer);
    return buffer;
}

ID3D11Texture2D* D3DHelpers::CreateTexture(unsigned int width, unsigned int height,  D3D11_BIND_FLAG bindFlags, RenderingQuality *quality)
{
    D3D11_TEXTURE2D_DESC textureDesc = { 0 };

    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.Format = quality->TextureFormat;
    textureDesc.SampleDesc.Quality = quality->Quality;
    textureDesc.SampleDesc.Count = quality->SampleCount;
    textureDesc.MipLevels = quality->MipLevels;

    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = bindFlags;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    textureDesc.ArraySize = 1;

    ID3D11Texture2D* texture;
    Res::Get()->Device->CreateTexture2D(&textureDesc, NULL, &texture);
    return texture;
}

ID3D11RenderTargetView* D3DHelpers::CreateRenderTarget(ID3D11Resource *resource, D3D11_RTV_DIMENSION dimensions, RenderingQuality *quality)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.ViewDimension = dimensions;
    renderTargetViewDesc.Format = quality->TextureFormat;

    ID3D11RenderTargetView* target;
    // resource is the back buffer i.e.
    Res::Get()->Device->CreateRenderTargetView(resource, &renderTargetViewDesc, &target);
    return target;
}

ID3D11DepthStencilView* D3DHelpers::CreateDepthStencilView()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    depthStencilDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    depthStencilDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    depthStencilDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    depthStencilDesc.Width = Res::Get()->ViewPort->Width;
    depthStencilDesc.Height = Res::Get()->ViewPort->Height;

    ID3D11Texture2D *depthStencil;
    Res::Get()->Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    ID3D11DepthStencilView *depthstencilView;
    Res::Get()->Device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthstencilView);
    return depthstencilView;
}

ID3D11ShaderResourceView* D3DHelpers::CreateRandom1DTexture(unsigned int size)
{
    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = nullptr;
    data.SysMemPitch = sizeof(XMFLOAT4) * size;
    data.SysMemSlicePitch = 0;

    D3D11_TEXTURE1D_DESC textureDesc = { 0 };
    textureDesc.Width = 1024;
    textureDesc.MiscFlags = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.ArraySize = 1;

    ID3D11Texture1D* texture;
    Res::Get()->Device->CreateTexture1D(&textureDesc, &data, &texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
    resourceDesc.Format = textureDesc.Format;
    resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    resourceDesc.Texture1D.MipLevels = textureDesc.MipLevels;
    resourceDesc.Texture1D.MostDetailedMip = 0;

    ID3D11ShaderResourceView* resource;
    Res::Get()->Device->CreateShaderResourceView(texture, &resourceDesc, &resource);
    return resource;
}

ID3D11ShaderResourceView* D3DHelpers::CreateRandom2DTexture(unsigned int width, unsigned int height)
{
    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = nullptr;
    data.SysMemPitch = sizeof(XMFLOAT4) * width * height;
    data.SysMemSlicePitch = 0;

    D3D11_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MiscFlags = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.ArraySize = 1;

    ID3D11Texture2D* texture;
    Res::Get()->Device->CreateTexture2D(&textureDesc, &data, &texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
    resourceDesc.Format = textureDesc.Format;
    resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceDesc.Texture2D.MipLevels = textureDesc.MipLevels;
    resourceDesc.Texture2D.MostDetailedMip = 0;

    ID3D11ShaderResourceView* resource;
    Res::Get()->Device->CreateShaderResourceView(texture, &resourceDesc, &resource);
    return resource;
}

float D3DHelpers::RandomFloat(float min, float max)
{
    auto r = rand();
    auto random = (float)(r) / (float)RAND_MAX;
    return min + random * (max - min);
}

XMFLOAT4* D3DHelpers::RandomXMFLOAT4()
{
    auto x = D3DHelpers::RandomFloat(-1.0f, 1.0f);
    auto y = D3DHelpers::RandomFloat(-1.0f, 1.0f);
    auto z = D3DHelpers::RandomFloat(-1.0f, 1.0f);
    return new XMFLOAT4(x, y, z, 0.0f);
}