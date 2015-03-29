#include "stdafx.h"
#include "deferredtarget.h"


DeferredTarget::DeferredTarget(RenderingQuality *quality)
{
    _quality = quality;
    _renderTargetView = (ID3D11RenderTargetView*)malloc(sizeof(ID3D11RenderTargetView) * BUFFER_COUNT);
    _renderTargetTexture = (ID3D11Texture2D*)malloc(sizeof(ID3D11Texture2D) * BUFFER_COUNT);

    auto bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    auto texture = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, quality);
    auto renderTarget = D3DHelpers::CreateRenderTarget(texture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);
    _renderTargetTexture = texture;
    _renderTargetView = renderTarget;
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


void DeferredTarget::CreateRenderTarget(ID3D11Resource *buffer)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    renderTargetViewDesc.Format = Res::Get()->RenderQuality->DephStencilTextureFormat;

    ID3D11RenderTargetView *targetView;
    Res::Get()->Device->CreateRenderTargetView(buffer, &renderTargetViewDesc, &targetView);
    SetRenderTargetView(targetView);
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
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    auto bindFlags = D3D11_BIND_DEPTH_STENCIL;
    _depthStencilBuffer = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, _quality, DXGI_FORMAT_D24_UNORM_S8_UINT);

    Res::Get()->Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
}