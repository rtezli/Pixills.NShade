#pragma once

#include "res.h"
#include "light.h"

class PointLight : public Light  
{
public:
    void SetPosition(XMFLOAT3 *position);
    void SetColorIntensity(XMFLOAT4 *colorIntesity);
    ID3D11Buffer* GetBuffer(){ return _positionBuffer; };
private:
    ID3D11Buffer    *_positionBuffer;
    XMFLOAT3        *_position;
    XMFLOAT3        *_color;
    float           _intensity;
};