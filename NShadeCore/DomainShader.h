#pragma once

#include "res.h"
#include "file.h"
#include "node.h"
#include "export.h"
#include "shader.h"

EXTERN class API  DomainShader : public Node, public Shader
{
public:
    static DomainShader* Load(wchar_t *fileName);
private:
    DomainShader(ID3D11DomainShader *shader);
    ID3D11DomainShader *_shader;
};