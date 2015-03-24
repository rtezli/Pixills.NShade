#include "stdafx.h"
#include "immediatetarget.h"


ImmediateTarget::ImmediateTarget()
{
    auto width = Res::Get()->ViewPort->Width;
    auto height = Res::Get()->ViewPort->Height;
    auto bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    auto q = Res::Get()->RenderQuality;

    auto quality = new RenderingQuality();
    quality->Quality = Res::Get()->RenderQuality->Quality;
    quality->SampleCount = Res::Get()->RenderQuality->SampleCount;
    quality->MipLevels = Res::Get()->RenderQuality->MipLevels;
    quality->BufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
    quality->TextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

    auto texture = D3DHelpers::CreateTexture(width, height, (D3D11_BIND_FLAG)bindFlags, quality);
    auto renderTarget = D3DHelpers::CreateRenderTarget(texture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);
    _renderTargetTexture = texture;
    _renderTargetView = renderTarget;
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

