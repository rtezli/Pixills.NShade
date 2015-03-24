#include "stdafx.h"
#include "deferredbuffer.h"


DeferredBuffer::DeferredBuffer()
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


    for (unsigned int i = 0; i < BUFFER_COUNT; i++)
    {
        auto texture = D3DHelpers::CreateTexture(width, height, (D3D11_BIND_FLAG)bindFlags, quality);
        auto renderTarget = D3DHelpers::CreateRenderTarget(texture, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);
        _renderTargetTextureArray[i] = texture;
        _renderTargetViewArray[i] = renderTarget;
    }
}