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

void HullShader::Render()
{
    auto buffers = GetBuffers();
    if (buffers)
    {
        for (unsigned int b = 0; b < buffers->size(); b++)
        {
            auto buffer = buffers->at(b);

            Res::Get()->DeviceContext->HSSetConstantBuffers(b, 1, &buffer);
        }
    }

    auto resources = GetResources();
    if (resources)
    {
        for (unsigned int r = 0; r < resources->size(); r++)
        {
            auto resource = resources->at(r);
            Res::Get()->DeviceContext->HSSetShaderResources(r, 1, &resource);
        }
    }
    Res::Get()->DeviceContext->HSSetShader(_shader, NULL, 0);
}