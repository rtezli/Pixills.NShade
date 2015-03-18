#pragma once

#include "res.h"
#include "file.h"
#include "node.h"

class HullShader : public Node
{
public:
    static HullShader* Load(wchar_t *fileName);
private:
    HullShader(ID3D11HullShader *shader);
    ID3D11HullShader *_shader;
};