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