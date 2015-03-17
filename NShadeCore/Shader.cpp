#include "stdafx.h"
#include "shader.h"

HRESULT Shader::SetVertexShader(wchar_t *compiledShaderFile)
{
    auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
    auto result = Res::Get()->Device->CreateVertexShader(vsByteCode->Bytes, vsByteCode->Length, NULL, &Res::Get()->Shaders->VertexShader);

    if (FAILED(result))
    {
        return result;
    }

    static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,      D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex position
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex color
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0,      D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Normal vector
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0,         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Texture UV

        { "COLOR",      1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Ambient Light color where w is intensity
        { "POSITION",   1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }  // Point light position w is intensity
    };

    return  Res::Get()->Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->Bytes, vsByteCode->Length, &Res::Get()->InputLayout);
}

HRESULT Shader::CompileVertexShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11VertexShader* vertexShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

HRESULT Shader::SetHullShader(wchar_t *compiledShaderFile)
{
    auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
    auto shaders = Res::Get()->Shaders;
    return Res::Get()->Device->CreateHullShader(hsByteCode->Bytes, hsByteCode->Length, NULL, &shaders->HullShader);
}

HRESULT Shader::CompileHullShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11HullShader* hullShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, HS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &hullShader);
}

HRESULT Shader::SetGeometryShader(wchar_t *compiledShaderFile)
{
    auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
    auto shaders = Res::Get()->Shaders;
    return Res::Get()->Device->CreateGeometryShader(gsByteCode->Bytes, gsByteCode->Length, NULL, &shaders->GeometryShader);
}

HRESULT Shader::CompileGeometryShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11GeometryShader* geometryShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, GS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &geometryShader);
}

HRESULT Shader::SetPixelShader(wchar_t *compiledShaderFile)
{
    auto psByteCode = File::ReadFileBytes(compiledShaderFile);
    return Res::Get()->Device->CreatePixelShader(psByteCode->Bytes, psByteCode->Length, NULL, &Res::Get()->Shaders->PixelShader);
}

HRESULT Shader::CompilePixelShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11PixelShader* pixelShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, PS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
}

HRESULT Shader::CompileShader(wchar_t *compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile)
{
    ID3DBlob* shaderBlob = 0;
    unsigned int flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE",
        "1",
        NULL,
        NULL
    };

    return D3DCompileFromFile(compiledShaderFile,
        NULL, // Defines
        D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", // Entrypoint
        shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}
