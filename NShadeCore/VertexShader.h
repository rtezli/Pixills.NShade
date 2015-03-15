#pragma once

#include "res.h"
#include "file.h"

class VertexShader
{
public:
    ID3D11InputLayout*          const GetInputLayout(){ return _inputLayout; }
    ID3D11VertexShader*         const GetShader(){ return _vertexShader; }
    static VertexShader*        Load(wchar_t *fileName);
    static VertexShader*        Compile(wchar_t *sourceCode);
private:
    ID3D11InputLayout           *_inputLayout;
    ID3D11VertexShader          *_vertexShader;
};