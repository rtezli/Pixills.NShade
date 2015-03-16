#pragma once

#include "res.h"
#include "light.h"
#include "constbufferprovider.h"

class PointLight : public ConstBufferProvider, public Light
{
public:
    void SetPosition(XMFLOAT3 *position);
    void SetColorIntensity(XMFLOAT4 *colorIntesity);
private:
    ID3D11Buffer    *_positionBuffer;
    XMFLOAT3        *_position;
    XMFLOAT3        *_color;
    float           _intensity;
};