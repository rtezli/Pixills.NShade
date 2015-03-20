#pragma once

#include "vector"
#include "res.h"
#include "export.h"

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

using namespace std;

EXTERN class API Shader
{
public:
    //Shader();
public:
    vector<ID3D11Buffer*>               const GetBuffers(){ return _buffers; }
    vector<ID3D11ShaderResourceView*>   const GetResources(){ return _resources; }
    ID3D11SamplerState*                 const GetSamplerState(){ return _samplerState; }
    void                                AddBuffer(ID3D11Buffer *buffer);
    void                                AddResource(ID3D11ShaderResourceView *resource);
protected:
    vector<ID3D11Buffer*>               _buffers;
    vector<ID3D11ShaderResourceView*>   _resources;
    ID3D11SamplerState*                 _samplerState;
};