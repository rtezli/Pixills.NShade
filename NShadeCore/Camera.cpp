#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	_deviceResources = resources;

	_hAngle = 0.0f;
	_vAngle = 0.0f;

	_radius = 6.0f;
	_hAngle = 0.0f;
	_vAngle = 0.0f;

	Initialize();
}

VOID Camera::Initialize()
{
	auto z = _radius * sin(_hAngle * -1);
	auto x = sqrt(pow(_radius, 2) - pow(z, 2));

	auto eyePos = new XMFLOAT4{ 0.0f, 4.0f, _radius, 0.0f };
	_eyePosition = shared_ptr<XMFLOAT4>(eyePos);

	auto focusPos = new XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };
	_focusPosition = shared_ptr<XMFLOAT4>(focusPos);

	auto upDir = new XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.0f };
	_upDirection = shared_ptr<XMFLOAT4>(upDir);

	auto wMatrix = XMMatrixTranspose(XMMatrixIdentity());

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, wMatrix);

	auto vMatrix = XMMatrixTranspose(
		XMMatrixLookAtRH(
			XMLoadFloat4(_eyePosition.get()),
			XMLoadFloat4(_focusPosition.get()),
			XMLoadFloat4(_upDirection.get()))
		);

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, vMatrix);

	FLOAT fovAngleY = GetFieldOfView();
	FLOAT aspectRatio = GetAspectRatio();

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 1.0f;
	}

	XMMATRIX	perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.1f, 100.0f);
	XMFLOAT4X4	orientation = ScreenRotation::Rotation0;
	XMMATRIX	orientationMatrix = XMLoadFloat4x4(&orientation);

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	auto cameraPosition = XMFLOAT4(_eyePosition->x, _eyePosition->y, _eyePosition->z, 0.0f);
	auto constBufferData = new ConstantBufferData{ world, view, projection };

	_constBufferData = shared_ptr<ConstantBufferData>(constBufferData);

	InitializeConstantBuffer();
	InitializePositionBuffer();
}

VOID Camera::SetPosition(XMFLOAT4* p)
{
	_focusPosition = shared_ptr<XMFLOAT4>(p);
}

VOID Camera::SetFocusPoint(XMFLOAT4* p)
{
	_eyePosition = shared_ptr<XMFLOAT4>(p);
}

VOID Camera::MoveTo(XMFLOAT4* point)
{
	auto world = GetConstBufferData()->World;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixTranslation(point->x, point->y, point->z)));
}

VOID Camera::Rotate(XMFLOAT3* point)
{
	auto moderationH = 0.001;
	auto moderationV = 0.009;

	_hAngle = _hAngle + point->x * moderationH;
	_vAngle = _vAngle + point->y * moderationV;

	auto world = GetConstBufferData()->World;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixRotationY(_hAngle)));
}

VOID Camera::InitializeConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	auto constBufferData = GetConstBufferData();

	D3D11_SUBRESOURCE_DATA constantBufferData;
	constantBufferData.pSysMem = constBufferData;
	constantBufferData.SysMemPitch = 0;
	constantBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* constBuffer;
	auto result = _deviceResources->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &constBuffer);
	_constBuffer = shared_ptr<ID3D11Buffer>(constBuffer);
}

VOID Camera::InitializePositionBuffer()
{
	D3D11_BUFFER_DESC positionBufferDesc;
	positionBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	positionBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	positionBufferDesc.CPUAccessFlags = 0;
	positionBufferDesc.MiscFlags = 0;
	positionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	D3D11_SUBRESOURCE_DATA positionBufferData;
	positionBufferData.pSysMem = GetPositionBufferData();
	positionBufferData.SysMemPitch = 0;
	positionBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* positionBuffer;
	auto result = _deviceResources->Device->CreateBuffer(&positionBufferDesc, &positionBufferData, &positionBuffer);
	_positionBuffer = shared_ptr<ID3D11Buffer>(positionBuffer);
}