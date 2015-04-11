#pragma once

#include "memory"
#include "processingstep.h"
#include "d3dhelpers.h"


class PostProcessor
{
public:
    void                    Render();
    void                    AddStep(ProcessingStep *step);
    static PostProcessor*   Create(RenderingQuality *quality);
private:
    PostProcessor(RenderingQuality *quality);
    void CreateDepthStencil();
    void CreateDepthStencilStateDescription();
private:
    shared_ptr<vector<ProcessingStep>>  _processingSteps;

    RenderingQuality                    *_quality;
    ID3D11Texture2D                     *_renderTarget;
    ID3D11RenderTargetView              *_renderTargetView;

    ID3D11Texture2D                     *_depthStencilBuffer;
    ID3D11DepthStencilView              *_depthStencilView;
    ID3D11DepthStencilState             *_depthStencilState;    
};