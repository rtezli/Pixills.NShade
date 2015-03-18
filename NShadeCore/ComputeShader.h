#pragma once

#include "res.h"
#include "file.h"
#include "node.h"

class ComputeShader : public Node
{
public:
    static ComputeShader* Load(wchar_t *fileName);
private:
    ComputeShader(ID3D11ComputeShader *shader);
    ID3D11ComputeShader *_shader;
};