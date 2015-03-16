#include "stdafx.h"
#include "pixelshader.h"

PixelShader* PixelShader::Load(wchar_t *fileName)
{
    auto ps = new PixelShader();
    auto vsByteCode = File::ReadFileBytes(fileName);
    auto shader = ps->GetShader();
    auto result = Res::Get()->Device->CreatePixelShader(vsByteCode->Bytes, vsByteCode->Length, NULL, &shader);
    return ps;
}

PixelShader* PixelShader::Compile(wchar_t *sourceCode)
{
    //ID3DBlob* shaderBlob = 0;
    //ID3D11VertexShader* vertexShader = 0;

    //auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
    //result = Res::Get()->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
    auto ps = new PixelShader();
    return ps;
}
