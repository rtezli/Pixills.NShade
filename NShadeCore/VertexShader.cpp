#include "stdafx.h"
#include "vertexshader.h"

VertexShader::VertexShader(ID3D11VertexShader *shader, ID3D11InputLayout *layout)
{
    _vertexShader = shader;
    _inputLayout = layout;
}

VertexShader* VertexShader::Load(wchar_t *fileName)
{
    auto vsByteCode = File::ReadFileBytes(fileName);
    
    ID3D11VertexShader *shader;
    Res::Get()->Device->CreateVertexShader(vsByteCode->Bytes, vsByteCode->Length, NULL, &shader);
    
    static const D3D11_INPUT_ELEMENT_DESC VertexDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    ID3D11InputLayout *layout;
    Res::Get()->Device->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc), vsByteCode->Bytes, vsByteCode->Length, &layout);
    return new VertexShader(shader, layout);
}

VertexShader* VertexShader::Compile(wchar_t *sourceCode)
{
    //ID3DBlob* shaderBlob = 0;
    //ID3D11VertexShader* vertexShader = 0;

    //auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
    //result = Res::Get()->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
    auto vs = new VertexShader(nullptr, nullptr);
    return vs;
}