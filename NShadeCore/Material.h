#pragma once

#include "common.h"
#include "shader.h"

EXTERN class API Material
{
public:
    XMFLOAT4                        Color;
    shared_ptr<ID3D10Texture2D>     Texture;
    shared_ptr<ID3D10Texture2D>     BumpMap;
    shared_ptr<ID3D10Texture2D>     DisplaceMentMap;
    shared_ptr<vector<Shader>>      Shaders;
    float                           SpecularPower;
};