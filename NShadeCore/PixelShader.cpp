#include "stdafx.h"
#include "pixelshader.h"

PixelShader::PixelShader(ID3D11PixelShader *shader)
{
    _shader = shader;
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

void PixelShader::Render()
{
    auto buffers = GetBuffers();
    if (buffers)
    {
        for (unsigned int b = 0; b < buffers->size(); b++)
        {
            auto buffer = buffers->at(b);

            Res::Get()->DeviceContext->PSSetConstantBuffers(b, 1, &buffer);
        }
    }

    auto resources = GetResources();
    if (resources)
    {
        for (unsigned int r = 0; r < resources->size(); r++)
        {
            auto resource = resources->at(r);
            Res::Get()->DeviceContext->PSSetShaderResources(r, 1, &resource);
        }
    }
    Res::Get()->DeviceContext->PSSetShader(_shader, NULL, 0);
}