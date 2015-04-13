#include "stdafx.h"
#include "renderer.h"

Renderer::Renderer(bool useSwapChain)
{
    _renderDeferred = false;
    _isInitialized = false;
    _useSwapChain = useSwapChain;
    _rasterizerUseMultiSampling = true;

    _swapChain = new SwapChain();
}

HRESULT Renderer::Initialize()
{
    _isInitialized = false;

    _swapChain = new SwapChain();

    auto result = _swapChain->Initialize();

    result = CreateRasterizer();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateViewPort();
    if (FAILED(result))
    {
        return result;
    }

    _isInitialized = true;
    return result;
}

HRESULT Renderer::CreateRasterizerDescription()
{
    auto isRightHand = false;

    _rasterizerDesc.AntialiasedLineEnable = _rasterizerUseMultiSampling;
    _rasterizerDesc.DepthBias = 0;
    _rasterizerDesc.DepthBiasClamp = 0.0f;
    _rasterizerDesc.DepthClipEnable = true;
    _rasterizerDesc.FillMode = D3D11_FILL_SOLID;

    _rasterizerDesc.MultisampleEnable = _rasterizerUseMultiSampling;
    _rasterizerDesc.ScissorEnable = false;
    _rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    _rasterizerDesc.CullMode = D3D11_CULL_BACK;//D3D11_CULL_NONE;// 
    _rasterizerDesc.FrontCounterClockwise = true;

    return 0;
}

HRESULT Renderer::CreateRasterizer()
{
    auto result = CreateRasterizerDescription();
    if (FAILED(result))
    {
        return result;
    }

    result = Res::Get()->Device->CreateRasterizerState(&_rasterizerDesc, &Res::Get()->RasterizerState);
    if (FAILED(result))
    {
        return result;
    }

    Res::Get()->DeviceContext->RSSetState(Res::Get()->RasterizerState);

    return 0;
}


HRESULT Renderer::CreateViewPort()
{
    auto viewPort = Res::Get()->ViewPort;
    Res::Get()->DeviceContext->RSSetViewports(1, viewPort);
    return 0;
}


void Renderer::ClearScene()
{
    _swapChain->ClearRenderTargets();
}

void Renderer::Render(Scene *scene)
{
    if (!_isInitialized)
    {
        return;
    }

    ClearScene();

    Res::Get()->DeviceContext->ClearRenderTargetView(_swapChain->GetBackBufferTarget(), Res::Get()->DefaultColor);

    /* Render each model in the scene */
    for (unsigned int m = 0; m < scene->GetModels()->size(); m++)
    {
        auto model = scene->GetModels()->at(m);
        auto material = model.GetMaterial();
        auto shaders = material->GetShaders();

        unsigned int offset = 0;

        auto vertexBuffer = model.GetVertexBuffer();
        auto strides = shaders->VertexShader->GetInputSize();
        auto indexBuffer = model.GetIndexBuffer();

        Res::Get()->DeviceContext->IASetInputLayout(shaders->VertexShader->GetInputLayout());
        Res::Get()->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
        Res::Get()->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        Res::Get()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        auto vertexShader = shaders->VertexShader;
        if (vertexShader)
        {
            vertexShader->Render();
        }

        auto hullShader = shaders->HullShader;
        if (hullShader)
        {
            hullShader->Render();
        }

        auto domainShader = shaders->DomainShader;
        if (domainShader)
        {
            domainShader->Render();
        }

        auto geometryShader = shaders->GeometryShader;
        if (geometryShader)
        {
            geometryShader->Render();
        }

        // To stream output

        auto pixelshader = shaders->PixelShader;
        if (pixelshader)
        {
            pixelshader->Render();
        }

        auto backBuffer = _swapChain->GetBackBufferTarget();
        auto depthStencil = _swapChain->GetDepthStencilView();
        Res::Get()->DeviceContext->OMSetRenderTargets(1, &backBuffer, depthStencil);
        Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }

    auto postProcessor = scene->GetPostProcessor();
    if (postProcessor)
    {
        postProcessor->Render();
    }

    if (!_swapChain)
    {
        return;
    }
    _swapChain->Present();
}

void Renderer::Tesselate(Shaders *shaders)
{
    if (!shaders->HullShader || !shaders->DomainShader)
    {
        return;
    }
}

void Renderer::Resize()
{
    Initialize();
}