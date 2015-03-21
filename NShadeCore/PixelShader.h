#pragma once

#include "res.h"
#include "file.h"
#include "export.h"
#include "shader.h"

EXTERN class API PixelShader : public Shader
{
public:
    static PixelShader*     Load(wchar_t *fileName);
    static PixelShader*     Compile(wchar_t *sourceCode);
    ID3D11PixelShader*      const GetShader(){ return _shader; }
    void                    Render();
private:
    PixelShader(ID3D11PixelShader *shader);
private:
    ID3D11PixelShader       *_shader;
};