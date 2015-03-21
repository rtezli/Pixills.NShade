#include "stdafx.h"
#include "domainshader.h"

DomainShader::DomainShader(ID3D11DomainShader *shader)
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

void DomainShader::Render()
{
    auto buffers = GetBuffers();
    if (buffers)
    {
        for (unsigned int b = 0; b < buffers->size(); b++)
        {
            auto buffer = buffers->at(b);

            Res::Get()->DeviceContext->DSSetConstantBuffers(b, 1, &buffer);
        }
    }

    auto resources = GetResources();
    if (resources)
    {
        for (unsigned int r = 0; r < resources->size(); r++)
        {
            auto resource = resources->at(r);
            Res::Get()->DeviceContext->DSSetShaderResources(r, 1, &resource);
        }
    }
    Res::Get()->DeviceContext->DSSetShader(_shader, NULL, 0);
}