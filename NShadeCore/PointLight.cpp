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
}