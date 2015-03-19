#pragma once

#include "res.h"
#include "file.h"
#include "ddstextureloader.h"
#include "wictextureloader.h"

class Texture
{
public:
    static Texture* Load(wchar_t *fileName);
private:
    Texture(ID3D11ShaderResourceView *resourceView);
    ID3D11ShaderResourceView* _resourceView;
};