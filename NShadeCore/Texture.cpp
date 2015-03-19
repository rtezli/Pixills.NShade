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