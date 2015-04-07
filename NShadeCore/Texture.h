#pragma once

#include "res.h"
#include "file.h"
#include "export.h"
#include "ddstextureloader.h"
#include "wictextureloader.h"

EXTERN class API Texture
{
public:
    static Texture*             Load(wchar_t *fileName);
    static Texture*             Load(ID3D11Texture2D *texture);
    ID3D11ShaderResourceView*   const GetResources(){ return _resourceView; };
private:
    Texture(ID3D11ShaderResourceView *resourceView);
    ID3D11ShaderResourceView* _resourceView;
};