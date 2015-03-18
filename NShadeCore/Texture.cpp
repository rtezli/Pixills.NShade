#include "stdafx.h"
#include "texture.h"


Texture::Texture(ID3D11ShaderResourceView *texture)
{
    _texture = texture;
}
 
Texture* Texture::Load(wchar_t *fileName)
{
    ID3D11Resource *texture;
    ID3D11ShaderResourceView *resourceView;
    CreateDDSTextureFromFile(Res::Get()->Device, fileName, &texture, &resourceView, 0, nullptr);
    return new Texture(resourceView);
}