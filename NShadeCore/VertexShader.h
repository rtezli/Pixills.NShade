#pragma once

#include "res.h"
#include "file.h"
#include "export.h"
#include "shader.h"

EXTERN class API VertexShader : public Shader
{
public:
    unsigned int                const GetInputSize(){ return sizeof(NVertex); }
    ID3D11InputLayout*          const GetInputLayout(){ return _inputLayout; }
    ID3D11VertexShader*         const GetShader(){ return _shader; }
    static VertexShader*        Load(wchar_t *fileName);
    static VertexShader*        Compile(wchar_t *sourceCode);
    void                        Render();
private:
    VertexShader(ID3D11VertexShader *shader, ID3D11InputLayout *layout);
    unsigned int                    *_strides;
    ID3D11InputLayout               *_inputLayout;
    ID3D11VertexShader              *_shader;
};