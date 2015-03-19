#pragma once

#include "res.h"
#include "file.h"
#include "node.h"
#include "export.h"
#include "shader.h"

EXTERN class API  GeometryShader : public Node, public Shader
{
public:
    static GeometryShader* Load(wchar_t *fileName);
private:
    GeometryShader(ID3D11GeometryShader *shader);
    ID3D11GeometryShader *_shader;
};
