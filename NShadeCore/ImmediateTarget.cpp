#include "stdafx.h"
#include "immediatetarget.h"

ImmediateTarget::ImmediateTarget(RenderingQuality *quality)
{
    _quality = quality;

    //auto bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    //auto quality = new RenderingQuality();
    //quality->Quality = Res::Get()->RenderQuality->Quality;
    //quality->SampleCount = Res::Get()->RenderQuality->SampleCount;
    //quality->MipLevels = Res::Get()->RenderQuality->MipLevels; 
    //quality->TextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
    //quality->BufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

    //_renderTargetTexture = D3DHelpers::CreateTexture(width, height, (D3D11_BIND_FLAG)bindFlags, quality);
    //_renderTargetView = D3DHelpers::CreateRenderTarget(_renderTargetTexture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);

    //bindFlags = D3D11_BIND_DEPTH_STENCIL;
    //quality->TextureFormat = DXGI_FORMAT_D32_FLOAT;
    
}

ImmediateTarget* ImmediateTarget::Create(RenderingQuality *quality)
{
    auto target = new ImmediateTarget(quality);
    target->CreateDepthStencil();
    return target;
}

void ImmediateTarget::Render()
{

}


void ImmediateTarget::SetRenderTargets()
{
    Res::Get()->DeferredContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    Res::Get()->DeferredContext->RSSetViewports(1, Res::Get()->ViewPort);
}

void ImmediateTarget::ClearRenderTargets()
{
    Res::Get()->DeferredContext->ClearRenderTargetView(_renderTargetView, Res::Get()->DefaultColor);
    Res::Get()->DeferredContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void ImmediateTarget::CreateRenderTarget(ID3D11Resource *buffer)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    ID3D11RenderTargetView *targetView;
    Res::Get()->Device->CreateRenderTargetView(buffer, &renderTargetViewDesc, &targetView);
    SetRenderTargetView(targetView);
}

void ImmediateTarget::CreateDepthStencilStateDescription()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilStateDesc.StencilEnable = true;
    depthStencilStateDesc.StencilReadMask = 0xFF;
    depthStencilStateDesc.StencilWriteMask = 0xFF;

    depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    Res::Get()->Device->CreateDepthStencilState(&depthStencilStateDesc, &_depthStencilState);
    Res::Get()->DeviceContext->OMSetDepthStencilState(_depthStencilState, 1);
}

void ImmediateTarget::CreateDepthStencil()
{
    CreateDepthStencilStateDescription();

    auto quality = new RenderingQuality();
    quality->Quality = Res::Get()->RenderQuality->Quality;
    quality->SampleCount = Res::Get()->RenderQuality->SampleCount;
    quality->MipLevels = Res::Get()->RenderQuality->MipLevels;
    quality->TextureFormat = DXGI_FORMAT_D32_FLOAT;
    quality->BufferFormat = DXGI_FORMAT_D32_FLOAT ;

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = quality->BufferFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    auto width = Res::Get()->ViewPort->Width;
    auto height = Res::Get()->ViewPort->Height;
    auto bindFlags = D3D11_BIND_DEPTH_STENCIL;

    _depthStencilBuffer = D3DHelpers::CreateTexture(width, height, (D3D11_BIND_FLAG)bindFlags, quality);
    Res::Get()->Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
}

