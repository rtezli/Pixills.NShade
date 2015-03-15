#pragma once

#include "memory"
#include "d3d11.h"
#include "export.h"
#include "res.h"

using namespace std;

EXTERN class API Material
{
public:
    ShaderSet*                      const GetShaders(){ return _shaders.get(); };
private:
    XMFLOAT4                        _color;
    shared_ptr<ID3D11Texture2D>     _texture;
    shared_ptr<ID3D11Texture2D>     _bumpMap;
    shared_ptr<ID3D11Texture2D>     _displaceMentMap;
    shared_ptr<ShaderSet>           _shaders;
    float                           _specularPower;
};