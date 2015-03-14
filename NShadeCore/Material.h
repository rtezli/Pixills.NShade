#pragma once

#include "memory"
#include "d3d11.h"
#include "shader.h"

using namespace std;

EXTERN class API Material
{
public:
    XMFLOAT4                        Color;
    shared_ptr<ID3D11Texture2D>     Texture;
    shared_ptr<ID3D11Texture2D>     BumpMap;
    shared_ptr<ID3D11Texture2D>     DisplaceMentMap;
    shared_ptr<vector<Shader>>      Shaders;
    float                           SpecularPower;
};