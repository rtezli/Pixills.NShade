#pragma once

#include "common.h"

EXTERN class API Camera
{
public:
    VOID    Initialize();
    VOID    Move(POINT* p);
    VOID    Rotate(POINT* p);
    VOID    Update();
    FLOAT   GetAspectRatio(){ return Res::Get()->ViewPort->Width / Res::Get()->ViewPort->Height; };
    FLOAT   GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
    XMFLOAT4X4  *_worldMatrix, *_viewMatrix, *_projectionMatrix;
    XMFLOAT3    *_eyePosition, *_focusPosition, *_upDirection;
    FLOAT        _hAngle, _vAngle, _radius;
};