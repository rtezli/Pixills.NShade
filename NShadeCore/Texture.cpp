#include "stdafx.h"
#include "texture.h"


Texture::Texture(ID3D11ShaderResourceView *resourceView)
{
    _resourceView = resourceView;
}

Texture* Texture::Load(wchar_t *fileName)
{
    ID3D11ShaderResourceView *resourceView = nullptr;
    auto result = CreateDDSTextureFromFile(Res::Get()->Device, fileName, nullptr, &resourceView, 0);
    return new Texture(resourceView);
}

Texture* Texture::Load(ID3D11Texture2D *texture)
{
    D3D11_TEXTURE2D_DESC d;
    texture->GetDesc(&d);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    desc.Format = d.Format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    desc.Texture2D.MipLevels = d.MipLevels;
    desc.Texture2D.MostDetailedMip = 0;

    ID3D11ShaderResourceView *resourceView = nullptr;
    auto result = Res::Get()->Device->CreateShaderResourceView(texture, &desc, &resourceView);
    return new Texture(resourceView);
}