#pragma once

#include "res.h"
#include "file.h"

class PixelShader
{
public:
    static PixelShader* Load(wchar_t *fileName);
    static PixelShader* Compile(wchar_t *sourceCode);
    ID3D11PixelShader*  const GetShader(){ return _pixelShader; }
private:
    ID3D11PixelShader          *_pixelShader;
};