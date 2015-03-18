#include "stdafx.h"
#include "hullshader.h"

HullShader::HullShader(ID3D11HullShader* shader)
{
    _shader = shader;
}

HullShader* HullShader::Load(wchar_t *fileName)
{
    auto hsByteCode = File::ReadFileBytes(fileName);

    ID3D11HullShader *shader;
    Res::Get()->Device->CreateHullShader(hsByteCode->Bytes, hsByteCode->Length, NULL, &shader);
    return new HullShader(shader);
}