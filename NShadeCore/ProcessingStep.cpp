#include "stdafx.h"
#include "processingstep.h"

void ProcessingStep::AssignShaders(Shaders *texture)
{
    _shaders = shared_ptr<Shaders>(texture);

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

ID3D11Texture2D* ProcessingStep::ApplyOn(ID3D11Texture2D *texture)
{
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    _resourceDescription.Format = desc.Format;
    _resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    _resourceDescription.Texture2D.MipLevels = desc.MipLevels;
    _resourceDescription.Texture2D.MostDetailedMip = 0;

    ID3D11ShaderResourceView *resource;
    Res::Get()->Device->CreateShaderResourceView(texture, &_resourceDescription, &resource);
    Res::Get()->DeviceContext->VSSetShaderResources(0, 1, &resource);

    auto vs = _shaders->VertexShader;
    Res::Get()->DeviceContext->VSSetShader(vs->GetShader(), NULL, 0);

    auto ps = _shaders->PixelShader;
    auto state = ps->GetSamplerState();
    Res::Get()->DeviceContext->PSSetSamplers(0, 1, &state);
    Res::Get()->DeviceContext->PSSetShader(ps->GetShader(), NULL, 0);

    return texture;
}