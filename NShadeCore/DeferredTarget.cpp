#include "stdafx.h"
#include "deferredtarget.h"

DeferredTarget::DeferredTarget(RenderingQuality *quality)
{
    _quality = quality;

    auto bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    _renderTargetTexture = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, quality);
    _renderTargetView = D3DHelpers::CreateRenderTarget(_renderTargetTexture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);

    _backBufferTexture = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, quality);
    _backBufferView = D3DHelpers::CreateRenderTarget(_renderTargetTexture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);
}

DeferredTarget* DeferredTarget::Create(RenderingQuality *quality)
{
    auto target = new DeferredTarget(quality);
    target->CreateDepthStencil();
    return target;
}

void DeferredTarget::Render()
{

}

void DeferredTarget::Prepare()
{
    // set blank render
    // set current render target
}

void DeferredTarget::SetRenderTargets()
{
    Res::Get()->DeviceContext->RSSetViewports(1, Res::Get()->ViewPort);
}

void DeferredTarget::ClearRenderTargets()
{
    Res::Get()->DeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    Res::Get()->DeviceContext->ClearRenderTargetView(_renderTargetView, Res::Get()->DefaultColor);
    Res::Get()->DeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeferredTarget::CreateDepthStencilStateDescription()
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

void DeferredTarget::CreateDepthStencil()
{
    CreateDepthStencilStateDescription();

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = _quality->BufferFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    auto bindFlags = D3D11_BIND_DEPTH_STENCIL;
    _depthStencilBuffer = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, _quality, _quality->BufferFormat);

    Res::Get()->Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
}

ID3D11Texture2D* DeferredTarget::GetRenderTargetAsResource()
{
    return nullptr;
}