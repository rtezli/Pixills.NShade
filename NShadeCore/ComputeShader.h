#pragma once

#include "res.h"
#include "file.h"
#include "node.h"
#include "export.h"
#include "shader.h"

EXTERN class API ComputeShader : public Node, public Shader
{
public:
    static ComputeShader* Load(wchar_t *fileName);
private:
    ComputeShader(ID3D11ComputeShader *shader);
    ID3D11ComputeShader *_shader;
};