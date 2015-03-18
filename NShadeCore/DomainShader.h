#pragma once

#include "res.h"
#include "file.h"
#include "node.h"

class DomainShader : public Node
{
public:
    static DomainShader* Load(wchar_t *fileName);
private:
    DomainShader(ID3D11DomainShader *shader);
    ID3D11DomainShader *_shader;
};