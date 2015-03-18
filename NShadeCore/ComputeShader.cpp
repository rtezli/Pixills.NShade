#include "stdafx.h"
#include "computeshader.h"

ComputeShader::ComputeShader(ID3D11ComputeShader *shader)
{
    _shader = shader;
}

ComputeShader* ComputeShader::Load(wchar_t *fileName)
{
    auto csByteCode = File::ReadFileBytes(fileName);

    ID3D11ComputeShader *shader;
    Res::Get()->Device->CreateComputeShader(csByteCode->Bytes, csByteCode->Length, NULL, &shader);
    return new ComputeShader(shader);
}
