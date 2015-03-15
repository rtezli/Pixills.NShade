#include "stdafx.h"
#include "vertexshader.h"

VertexShader* VertexShader::Load(wchar_t *fileName)
{
    static const D3D11_INPUT_ELEMENT_DESC VertexDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex position
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex color
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Normal vector
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Texture UV
    };

    auto vs = new VertexShader();
    auto vsByteCode = File::ReadFileBytes(fileName);
    auto shader = vs->GetShader();
    auto result = Res::Get()->Device->CreateVertexShader(vsByteCode->Bytes, vsByteCode->Length, NULL, &shader);
    auto layout = vs->GetInputLayout();
    result = Res::Get()->Device->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc), vsByteCode->Bytes, vsByteCode->Length, &layout);
    return vs;
}

VertexShader* VertexShader::Compile(wchar_t *sourceCode)
{
    //ID3DBlob* shaderBlob = 0;
    //ID3D11VertexShader* vertexShader = 0;

    //auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
    //result = Res::Get()->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
    auto vs = new VertexShader();
    return vs;
}