#pragma once

#include "vertexshader.h"
#include "pixelshader.h"
#include "domainshader.h"
#include "hullshader.h"
#include "geometryshader.h"
#include "computeshader.h"

#ifndef PS_PROFILE
#define PS_PROFILE {"ps_5_0" }
#endif

#ifndef VS_PROFILE
#define VS_PROFILE {"vs_5_0" }
#endif

#ifndef GS_PROFILE
#define GS_PROFILE { "ps_5_0" }
#endif

#ifndef HS_PROFILE
#define HS_PROFILE { "hs_5_0" }
#endif

#ifndef DS_PROFILE
#define DS_PROFILE { "ds_5_0" }
#endif

enum ShaderVersion : char
{
    V0 = 0,
    V1 = 0,
    V2 = 0,
    V3 = 0,
    V4 = 0,
    V5 = 0
};

enum ShaderType : char
{
    VS = 1,
    PS = 2,
    GS = 3,
    HS = 4,
    CS = 5,
    DS = 6
};

struct Shaders
{
    PixelShader     *PixelShader;
    VertexShader    *VertexShader;
    HullShader      *HullShader;
    DomainShader    *DomainShader;
    GeometryShader  *GeometryShader;
    ComputeShader   *ComputeShader;
};