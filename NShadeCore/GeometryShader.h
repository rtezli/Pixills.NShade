#pragma once

#include "res.h"
#include "file.h"
#include "export.h"
#include "shader.h"

EXTERN class API  GeometryShader :  public Shader
{
public:
    static GeometryShader*  Load(wchar_t *fileName);
    void                    Render();
private:
    GeometryShader(ID3D11GeometryShader *shader);
    ID3D11GeometryShader *_shader;
};
