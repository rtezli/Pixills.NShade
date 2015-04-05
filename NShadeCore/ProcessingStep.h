#pragma once

#include "res.h"
#include "shaders.h"
#include "d3dhelpers.h"

class ProcessingStep
{
public:
    void                AssignShaders(Shaders *shaders);
    void                Render(ID3D11Texture2D *texture, unsigned int indexCount);
    void                Finalize(ID3D11Texture2D *source, ID3D11RenderTargetView *target, ID3D11DepthStencilView *depthStencil, unsigned int indexCount);
private:
    shared_ptr<Shaders>             _shaders;
    D3D11_SAMPLER_DESC              _samplerDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC _resourceDescription;
};