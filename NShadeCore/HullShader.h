#pragma once

#include "res.h"
#include "file.h"
#include "shader.h"

class HullShader : public Shader
{
public:
    static HullShader*  Load(wchar_t *fileName);
    void                Render();
private:
    HullShader(ID3D11HullShader *shader);
    ID3D11HullShader *_shader;
};