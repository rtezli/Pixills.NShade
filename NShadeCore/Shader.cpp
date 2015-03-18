#include "stdafx.h"
#include "shader.h"

Shader:Shader()
{
    _inputs = new vector<ID3D11Buffer*>();
}

void Shader::AddInput(ID3D11Buffer *buffer)
{
    _inputs->push_back(buffer);
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