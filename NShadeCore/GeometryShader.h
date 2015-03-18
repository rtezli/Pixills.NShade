#pragma once

#include "res.h"
#include "file.h"
#include "node.h"

class GeometryShader : public Node
{
public:
    static GeometryShader* Load(wchar_t *fileName);
private:
    GeometryShader(ID3D11GeometryShader *shader);
    ID3D11GeometryShader *_shader;
};
