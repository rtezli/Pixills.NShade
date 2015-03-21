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
    auto result = Res::Get()->Device->CreateBuffer(&bufferDesc, &bufferData, &buffer);
    if (FAILED(result))
    {
       // ???
    }
    return buffer;
}

ID3D11Texture2D* D3DHelpers::CreateTexture(unsigned int width, unsigned int height, D3D11_BIND_FLAG bindFlags, RenderingQuality *quality)
{
    D3D11_TEXTURE2D_DESC textureDesc = { 0 };

    textureDesc.Width = (unsigned int)Res::Get()->ViewPort->Width;
    textureDesc.Height = (unsigned int)Res::Get()->ViewPort->Height;
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
    auto texture = D3DHelpers::CreateTexture(
        Res::Get()->ViewPort->Width,
        Res::Get()->ViewPort->Height,
        D3D11_BIND_DEPTH_STENCIL,
        Res::Get()->RenderQuality);

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.ViewDimension = dimensions;
    renderTargetViewDesc.Format = Res::Get()->RenderQuality->TextureFormat;

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