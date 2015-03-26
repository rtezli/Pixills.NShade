#include "stdafx.h"
#include "immediatetarget.h"

ImmediateTarget::ImmediateTarget(RenderingQuality *quality)
{
    auto q = new RenderingQuality();
    q->Width = quality->Width;
    q->Height = quality->Height;
    q->Quality = quality->Quality;
    q->SampleCount = quality->SampleCount;
    q->MipLevels = quality->MipLevels;
    q->TextureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    q->BufferFormat = DXGI_FORMAT_D32_FLOAT;

    _quality = q;
    
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
    renderTargetViewDesc.Format = _quality->TextureFormat;

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

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = _quality->BufferFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    auto bindFlags = D3D11_BIND_DEPTH_STENCIL;
    auto tf = _quality->TextureFormat;
    _quality->TextureFormat = _quality->BufferFormat;
    _depthStencilBuffer = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, _quality);
    _quality->TextureFormat = tf;
    Res::Get()->Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
}