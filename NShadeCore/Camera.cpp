#include "stdafx.h"
#include "camera.h"

VOID Camera::Initialize()
{
    _worldMatrix = new XMFLOAT4X4();
    _viewMatrix = new XMFLOAT4X4();
    _projectionMatrix = new XMFLOAT4X4();

    _radius = 6.0f;
    _hAngle = 0.0f;
    _vAngle = 0.0f;

    auto z = _radius * sin(_hAngle * -1);
    auto x = sqrt(pow(_radius, 2) - pow(z, 2));

    _eyePosition = new XMFLOAT3{ 0.0f, 4.0f, _radius };
    _focusPosition = new XMFLOAT3{ 0.0f, 0.0f, 0.0f };
    _upDirection = new XMFLOAT3{ 0.0f, 1.0f, 0.0f };

    auto wMatrix = XMMatrixTranspose(XMMatrixIdentity());
    XMStoreFloat4x4(_worldMatrix, wMatrix);

    auto vMatrix = XMMatrixTranspose(
        XMMatrixLookAtRH(
        XMLoadFloat3(_eyePosition),
        XMLoadFloat3(_focusPosition),
        XMLoadFloat3(_upDirection)
        )
        );

    XMStoreFloat4x4(_viewMatrix, vMatrix);

    auto fovAngleY = GetFieldOfView();
    auto aspectRatio = GetAspectRatio();

    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 1.0f;
    }

    auto perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.1f, 100.0f);
    auto orientation = ScreenRotation::Rotation0;
    auto orientationMatrix = XMLoadFloat4x4(&orientation);

    XMStoreFloat4x4(_projectionMatrix, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

    Update();
}

VOID Camera::Move(POINT* p)
{
    XMStoreFloat4x4(_worldMatrix, XMMatrixTranspose(XMMatrixTranslation(p->x, 0.0, p->y)));
    Update();
}

VOID Camera::Rotate(POINT* p)
{
    auto moderationH = 0.001;
    auto moderationV = 0.009;

    _hAngle = _hAngle + p->x * moderationH;
    _vAngle = _vAngle + p->y * moderationV;

    XMStoreFloat4x4(_worldMatrix, XMMatrixTranspose(XMMatrixRotationY(_hAngle)));
    Res::Get()->ConstBufferData->world = *_worldMatrix;
}

VOID Camera::Update()
{
    ConstantBufferData constBufferData = { *_worldMatrix, *_viewMatrix, *_projectionMatrix, *_eyePosition };
    Res::Get()->ConstBufferData = new ConstantBufferData(constBufferData);
    Res::Get()->ConstBufferData = Res::Get()->ConstBufferData;
}