#pragma once

#include "vertexshader.h"
#include "pixelshader.h"
#include "domainshader.h"
#include "hullshader.h"
#include "geometryshader.h"
#include "computeshader.h"

struct Shaders
{
    PixelShader     *PixelShader;
    VertexShader    *VertexShader;
    HullShader      *HullShader;
    DomainShader    *DomainShader;
    GeometryShader  *GeometryShader;
    ComputeShader   *ComputeShader;
};
