#include "stdafx.h"
#include "domainshader.h"

DomainShader::DomainShader(ID3D11DomainShader * shader)
{
    _shader = shader;
}

DomainShader* DomainShader::Load(wchar_t *fileName)
{
    auto dsByteCode = File::ReadFileBytes(fileName);

    ID3D11DomainShader *shader;
    Res::Get()->Device->CreateDomainShader(dsByteCode->Bytes, dsByteCode->Length, NULL, &shader);
    return new DomainShader(shader);
}