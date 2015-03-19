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
    ID3D11PixelShader*      const GetShader(){ return _pixelShader; }
    ID3D11Buffer*           const GetInput(){ return _input; }
    vector<ID3D11Buffer*>   const GetInputs(){ return _inputs; }
private:
    PixelShader(ID3D11PixelShader *shader);
private:
    ID3D11PixelShader       *_pixelShader;
    vector<ID3D11Buffer*>    _inputs;
    ID3D11Buffer*           _input;
};