#include "stdafx.h"
#include "immediatetarget.h"

ImmediateTarget::ImmediateTarget(RenderingQuality *quality)
{
    _quality = quality;
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

void ImmediateTarget::Prepare()
{

}

void ImmediateTarget::SetRenderTargets()
{
    Res::Get()->DeviceContext->RSSetViewports(1, Res::Get()->ViewPort);
}

void ImmediateTarget::ClearRenderTargets()
{
    Res::Get()->DeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    Res::Get()->DeviceContext->ClearRenderTargetView(_renderTargetView, Res::Get()->DefaultColor);
    Res::Get()->DeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
    _depthStencilBuffer = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, _quality, _quality->BufferFormat);

    Res::Get()->Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
}

ID3D11Texture2D* ImmediateTarget::GetRenderTargetAsResource()
{
    return nullptr;
}