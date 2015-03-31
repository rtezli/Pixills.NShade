#include "stdafx.h"
#include "PostProcessor.h"

PostProcessor* PostProcessor::Create(RenderingQuality *quality)
{
    return new PostProcessor(quality);
}

PostProcessor::PostProcessor(RenderingQuality *quality)
{
    _processingSteps = shared_ptr<vector<ProcessingStep>>(new vector<ProcessingStep>());

    auto bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    _renderTarget = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, quality);
    _renderTargetView = D3DHelpers::CreateRenderTarget(_renderTarget, D3D11_RTV_DIMENSION_TEXTURE2DMS, quality);
}

void PostProcessor::CreateDepthStencilStateDescription()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilStateDesc.StencilEnable = true;
    depthStencilStateDesc.StencilReadMask = 0xFF;
    depthStencilStateDesc.StencilWriteMask = 0xFF;

    depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    Res::Get()->Device->CreateDepthStencilState(&depthStencilStateDesc, &_depthStencilState);
    Res::Get()->DeviceContext->OMSetDepthStencilState(_depthStencilState, 1);
}

void PostProcessor::CreateDepthStencil()
{
    CreateDepthStencilStateDescription();

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = _quality->BufferFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    auto bindFlags = D3D11_BIND_DEPTH_STENCIL;
    _depthStencilBuffer = D3DHelpers::CreateTexture((D3D11_BIND_FLAG)bindFlags, _quality, _quality->BufferFormat);

    Res::Get()->Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
}

void PostProcessor::AddStep(ProcessingStep *step)
{
    _processingSteps->push_back(*step);
}

void PostProcessor::Render()
{
    Res::Get()->DeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    if (_processingSteps)
    {
        for (unsigned int p = 0; p < _processingSteps->size(); p++)
        {
            auto step = _processingSteps->at(p);
        }

        Res::Get()->DeviceContext->DrawAuto();
    }
}