#include "stdafx.h"
#include "processingstep.h"

void ProcessingStep::AssignShaders(Shaders *shaders)
{
    _shaders = shared_ptr<Shaders>(shaders);

    D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP;
    _samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    _samplerDesc.AddressU = addressMode;
    _samplerDesc.AddressV = addressMode;
    _samplerDesc.AddressW = addressMode;
    _samplerDesc.MipLODBias = 0.0f;
    _samplerDesc.MaxAnisotropy = 1;
    _samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    _samplerDesc.BorderColor[0] = 0;
    _samplerDesc.BorderColor[1] = 0;
    _samplerDesc.BorderColor[2] = 0;
    _samplerDesc.BorderColor[3] = 0;
    _samplerDesc.MinLOD = 0;
    _samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
}

void ProcessingStep::Render(ID3D11Texture2D *resource, unsigned int indexCount)
{
    D3D11_TEXTURE2D_DESC desc;
    resource->GetDesc(&desc);
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    
    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDescription;
    resourceDescription.Format = desc.Format;
    resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    resourceDescription.Texture2D.MipLevels = desc.MipLevels;
    resourceDescription.Texture2D.MostDetailedMip = 0;

    Res::Get()->Device->CreateShaderResourceView(resource, &resourceDescription, &_shaderResource);
    Res::Get()->DeviceContext->VSSetShaderResources(0, 1, &res);

    auto vertexShader = _shaders->VertexShader;
    if (vertexShader)
    {
        Res::Get()->DeviceContext->VSSetShader(vertexShader->GetShader(), NULL, 0);
    }

    auto pixelShader = _shaders->PixelShader;
    if (pixelShader)
    {
        auto state = pixelShader->GetSamplerState();
        if (state)
        {
            Res::Get()->DeviceContext->PSSetSamplers(0, 1, &state);
        }
        Res::Get()->DeviceContext->PSSetShader(pixelShader->GetShader(), NULL, 0);
    }
    Res::Get()->DeviceContext->DrawIndexed(indexCount, 0, 0);

    res = nullptr;
    Res::Get()->DeviceContext->VSSetShaderResources(0, 1, &res);
}

void ProcessingStep::Finalize(ID3D11Texture2D *source, ID3D11RenderTargetView *target, ID3D11DepthStencilView *depthStencil, unsigned int indexCount)
{
    D3D11_TEXTURE2D_DESC sDesc;
    source->GetDesc(&sDesc);
    sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDescription;
    resourceDescription.Format = sDesc.Format;
    resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    resourceDescription.Texture2D.MipLevels = sDesc.MipLevels;
    resourceDescription.Texture2D.MostDetailedMip = 0;

    ID3D11ShaderResourceView *res;
    Res::Get()->Device->CreateShaderResourceView(source, &resourceDescription, &res);
    Res::Get()->DeviceContext->VSSetShaderResources(0, 1, &res);

    auto vertexShader = _shaders->VertexShader;
    if (vertexShader)
    {
        Res::Get()->DeviceContext->VSSetShader(vertexShader->GetShader(), NULL, 0);
    }

    auto pixelShader = _shaders->PixelShader;
    if (pixelShader)
    {
        auto state = pixelShader->GetSamplerState();
        if (state)
        {
            Res::Get()->DeviceContext->PSSetSamplers(0, 1, &state);
        }
        Res::Get()->DeviceContext->PSSetShader(pixelShader->GetShader(), NULL, 0);
    }

    Res::Get()->DeviceContext->OMSetRenderTargets(1, &target, depthStencil);
    Res::Get()->DeviceContext->DrawIndexed(indexCount, 0, 0);
}