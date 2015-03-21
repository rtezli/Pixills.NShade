#pragma once

#include "res.h"
#include "shaders.h"

class ProcessingStep
{
public:
    void                AssignShaders(Shaders *shaders);
    ID3D11Texture2D*    Apply(ID3D11Texture2D *texture);
    ID3D11Texture2D*    Apply(ID3D11ShaderResourceView *resource);
private:
    shared_ptr<Shaders> _shaders;
};