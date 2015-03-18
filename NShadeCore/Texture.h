#pragma once

#include "res.h"
#include "file.h"
#include "ddstextureloader.h"

class Texture
{
public:
    static Texture* Load(wchar_t *fileName);
private:
    Texture(ID3D11ShaderResourceView *texture);
    ID3D11ShaderResourceView* _texture;
};