#include "stdafx.h"
#include "geometryshader.h"

GeometryShader::GeometryShader(ID3D11GeometryShader *shader)
{
    _shader = shader;
}

GeometryShader* GeometryShader::Load(wchar_t *fileName)
{
    auto gsByteCode = File::ReadFileBytes(fileName);

    ID3D11GeometryShader *shader;
    Res::Get()->Device->CreateGeometryShader(gsByteCode->Bytes, gsByteCode->Length, NULL, &shader);
    return new GeometryShader(shader);
}

void GeometryShader::Render()
{
    auto buffers = GetBuffers();
    if (buffers)
    {
        for (unsigned int b = 0; b < buffers->size(); b++)
        {
            auto buffer = buffers->at(b);

            Res::Get()->DeviceContext->GSSetConstantBuffers(b, 1, &buffer);
        }
    }

    auto resources = GetResources();
    if (resources)
    {
        for (unsigned int r = 0; r < resources->size(); r++)
        {
            auto resource = resources->at(r);
            Res::Get()->DeviceContext->GSSetShaderResources(r, 1, &resource);
        }
    }
    Res::Get()->DeviceContext->GSSetShader(_shader, NULL, 0);
}