#pragma once

#include "res.h"
#include "shaders.h"

class ProcessingStep
{
public:
    void                        AssignShaders(Shaders *shaders);
    ID3D11ShaderResourceView*   ApplyOn(ID3D11Texture2D *texture);
    ID3D11ShaderResourceView*   ApplyOn(ID3D11ShaderResourceView *resource);
private:
    shared_ptr<Shaders>             _shaders;
    D3D11_SAMPLER_DESC              _samplerDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC _resourceDescription;
};