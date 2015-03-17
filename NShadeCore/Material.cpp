#include "stdafx.h"
#include "Material.h"

void Material::AssignShaders(Shaders *shaders)
{
    _shaders = shared_ptr<Shaders>(shaders);
}

void Material::SetColor(XMFLOAT4 *color)
{
    D3D11_BUFFER_DESC colorBufferDesc = { 0 };
    colorBufferDesc.ByteWidth = sizeof(XMFLOAT4);
    colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA  colorBufferData = { 0 };
    colorBufferData.pSysMem = color;
    colorBufferData.SysMemPitch = 0;
    colorBufferData.SysMemSlicePitch = 0;

    Res::Get()->Device->CreateBuffer(&colorBufferDesc, &colorBufferData, &_colorBuffer);
}