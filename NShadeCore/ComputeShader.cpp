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

void ComputeShader::Render()
{
    auto buffers = GetBuffers();
    if (buffers)
    {
        for (unsigned int b = 0; b < buffers->size(); b++)
        {
            auto buffer = buffers->at(b);

            Res::Get()->DeviceContext->CSSetConstantBuffers(b, 1, &buffer);
        }
    }

    auto resources = GetResources();
    if (resources)
    {
        for (unsigned int r = 0; r < resources->size(); r++)
        {
            auto resource = resources->at(r);
            Res::Get()->DeviceContext->CSSetShaderResources(r, 1, &resource);
        }
    }
    Res::Get()->DeviceContext->CSSetShader(_shader, NULL, 0);
}