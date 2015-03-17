#include "stdafx.h"
#include "pointlight.h"

void PointLight::SetPosition(XMFLOAT3 *position)
{
    _position = position;
}

void PointLight::SetColorIntensity(XMFLOAT4 *colorIntesity)
{
    _color = new XMFLOAT3(colorIntesity->x, colorIntesity->y, colorIntesity->z);
    _intensity = colorIntesity->w;

    D3D11_BUFFER_DESC bufferDesc = { 0 };
    bufferDesc.ByteWidth = sizeof(XMFLOAT4);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA bufferData = { 0 };
    bufferData.pSysMem = colorIntesity;
    bufferData.SysMemPitch = 0;
    bufferData.SysMemSlicePitch = 0;

    Res::Get()->Device->CreateBuffer(&bufferDesc, &bufferData, &_buffer);
}