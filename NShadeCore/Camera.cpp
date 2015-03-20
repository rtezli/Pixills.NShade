#include "stdafx.h"
#include "camera.h"

Camera::Camera()
{
    Initialize();
}

void Camera::Initialize()
{
    _worldMatrix = new XMFLOAT4X4();
    _viewMatrix = new XMFLOAT4X4();
    _projectionMatrix = new XMFLOAT4X4();

    _radius = 6.0f;
    _hAngle = 0.0f;
    _vAngle = 0.0f;

    auto z = _radius * sin(_hAngle * -1);
    auto x = sqrt(pow(_radius, 2) - pow(z, 2));

    _eyePosition = new XMFLOAT4{ 0.0f, 4.0f, _radius, 0.0f };
    _focusPosition = new XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };
    _upDirection = new XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.0f };

    auto wMatrix = XMMatrixTranspose(XMMatrixIdentity());
    XMStoreFloat4x4(_worldMatrix, wMatrix);

    auto vMatrix = XMMatrixTranspose(
        XMMatrixLookAtRH(
        XMLoadFloat4(_eyePosition),
        XMLoadFloat4(_focusPosition),
        XMLoadFloat4(_upDirection)
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

void Camera::SetPosition(XMFLOAT3 *p)
{
    _eyePosition = new XMFLOAT4(p->x, p->y, p->z, 0.0f);
    Update();
}

void Camera::SetFocusPoint(XMFLOAT3 *p)
{
    _focusPosition = new XMFLOAT4(p->x, p->y, p->z, 0.0f);
    Update();
}

void Camera::Move(POINT *point)
{
    XMStoreFloat4x4(_worldMatrix, XMMatrixTranspose(XMMatrixTranslation(point->x, 0.0f, point->y)));
    Update();
}

void Camera::Rotate(POINT *point)
{
    auto moderationH = 0.001f;
    auto moderationV = 0.009f;

    _hAngle = _hAngle + point->x * moderationH;
    _vAngle = _vAngle + point->y * moderationV;

    XMStoreFloat4x4(_worldMatrix, XMMatrixTranspose(XMMatrixRotationY(_hAngle)));
    Update();
}

void Camera::Update()
{
    auto constBufferData = new ConstantBufferData{ *_worldMatrix, *_viewMatrix, *_projectionMatrix, *_eyePosition };

    D3D11_BUFFER_DESC constantBufferDesc = { 0 };
    constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
    constantBufferData.pSysMem = constBufferData;
    constantBufferData.SysMemPitch = 0;
    constantBufferData.SysMemSlicePitch = 0;

    ID3D11Buffer* matrixBuffer;
    Res::Get()->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &matrixBuffer);
    if (!_matrixBuffer)
    {
        _matrixBuffer = matrixBuffer;
    }
    else
    {
        memcpy(_matrixBuffer, matrixBuffer, sizeof(ConstantBufferData));
    }
}