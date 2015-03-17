#include "stdafx.h"
#include "pixelshader.h"

PixelShader::PixelShader(ID3D11PixelShader *shader)
{
    _pixelShader = shader;
}

PixelShader* PixelShader::Load(wchar_t *fileName)
{
    auto psByteCode = File::ReadFileBytes(fileName);
    ID3D11PixelShader *shader;
    Res::Get()->Device->CreatePixelShader(psByteCode->Bytes, psByteCode->Length, NULL, &shader);
    return new PixelShader(shader);
}

PixelShader* PixelShader::Compile(wchar_t *sourceCode)
{
    //ID3DBlob* shaderBlob = 0;
    //ID3D11VertexShader* vertexShader = 0;

    //auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
    //result = Res::Get()->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
    auto ps = new PixelShader(nullptr);
    return ps;
}
