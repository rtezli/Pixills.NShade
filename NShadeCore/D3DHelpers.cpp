#include "stdafx.h"
#include "d3dhelpers.h"

ID3D11Buffer* D3DHelpers::CreateBuffer(CHAR *data, UINT size, D3D11_BIND_FLAG bindFlags)
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
    auto result = Res::Get()->Device->CreateBuffer(&bufferDesc, &bufferData, &buffer);
    if (FAILED(result))
    {
       // ???
    }
    return buffer;
}

ID3D11Texture2D* D3DHelpers::CreateTexture(D3D11_BIND_FLAG bindFlags)
{
    D3D11_TEXTURE2D_DESC textureDesc = { 0 };

    textureDesc.Width = (UINT)Res::Get()->ViewPort->Width;
    textureDesc.Height = (UINT)Res::Get()->ViewPort->Height;
    textureDesc.Format = Res::Get()->RenderQuality->TextureFormat;
    textureDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    textureDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    textureDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;

    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = bindFlags;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    textureDesc.ArraySize = 1;

    ID3D11Texture2D* texture;
    auto result = Res::Get()->Device->CreateTexture2D(&textureDesc, NULL, &texture);
    if (FAILED(result))
    {
        // ???  return result;
    }
    return texture;
}

ID3D11RenderTargetView* D3DHelpers::CreateRenderTarget(ID3D11Resource *resource, D3D11_RTV_DIMENSION dimensions)
{
    auto texture = D3DHelpers::CreateTexture(D3D11_BIND_RENDER_TARGET);

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.ViewDimension = dimensions;
    renderTargetViewDesc.Format = Res::Get()->RenderQuality->TextureFormat;

    ID3D11RenderTargetView* target;
    // resource is the back buffer i.e.
    auto result = Res::Get()->Device->CreateRenderTargetView(resource, &renderTargetViewDesc, &target);
    if (FAILED(result))
    {
        // ???  return result;
    }
    return target;
}