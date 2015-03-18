#include "stdafx.h"
#include "texture.h"


Texture::Texture(ID3D11ShaderResourceView *texture)
{
    _texture = texture;
}
 
Texture* Texture::Load(wchar_t *fileName)
{
    unsigned int result;
    ID3D11ShaderResourceView *texture = 0;
    //D3DX11CreateTextureFromFile(Res::Get()->Device(), fileName, nullptr, nullptr, &texture, result);
    return new Texture(texture);
}