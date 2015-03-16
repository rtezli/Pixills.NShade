#pragma once

#include "res.h"
#include "constbufferprovider.h"

enum Perspective : char
{
    UNDEFINED = 0,
    FIRST_PERSON = 1,
    THIRD_PERSON = 2,
    ISOMETRIC = 3
};

EXTERN class API Camera : public ConstBufferProvider
{
public:
    Camera();
public:
    void    Initialize();
    void    SetPosition(XMFLOAT3 *position);
    void    SetFocusPoint(XMFLOAT3 *point);
    void    Rotate(POINT* p);
    void    Move(POINT* p);
    void    Update();
    float   GetAspectRatio(){ return Res::Get()->ViewPort->Width / Res::Get()->ViewPort->Height; };
    float   GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
    XMFLOAT4X4  *_worldMatrix, *_viewMatrix, *_projectionMatrix;
    XMFLOAT3    *_eyePosition, *_focusPosition, *_upDirection;
    float       _hAngle, _vAngle, _radius;
};