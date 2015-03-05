#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = resources;
	Initialize();
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	m_radius = 6.0f;
	m_hAngle = 0.0f;
	m_vAngle = 0.0f;

	auto z = m_radius * sin(m_hAngle * -1);
	auto x = sqrt(pow(m_radius, 2) - pow(z, 2));

	m_eyePosition = new XMFLOAT3{ 0.0f, 4.0f, m_radius };
	m_focusPosition = new XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_upDirection = new XMFLOAT3{ 0.0f, 1.0f, 0.0f };

	auto wMatrix = XMMatrixTranspose(XMMatrixIdentity());

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, wMatrix);

	auto vMatrix = XMMatrixTranspose(
		XMMatrixLookAtRH(
		XMLoadFloat3(m_eyePosition),
		XMLoadFloat3(m_focusPosition),
		XMLoadFloat3(m_upDirection)
		)
		);

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, vMatrix);

	float fovAngleY = GetFieldOfView();
	float aspectRatio = GetAspectRatio();

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 1.0f;
	}

	XMMATRIX	perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.1f, 100.0f);
	XMFLOAT4X4	orientation = ScreenRotation::Rotation0;
	XMMATRIX	orientationMatrix = XMLoadFloat4x4(&orientation);

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	auto eye = XMFLOAT4(m_eyePosition->x, m_eyePosition->y, m_eyePosition->z, 0.0f);
	auto constBufferData = new ConstantBufferData{ world, view, projection, eye };

	m_pConstBufferData = shared_ptr<ConstantBufferData>(constBufferData);

	InitializeConstantBuffer();
}

void Camera::Move(POINT* p)
{
	auto world = GetConstBufferData()->World;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixTranslation(p->x, 0.0, p->y)));
}

void Camera::Rotate(POINT* p)
{
	auto moderationH = 0.001;
	auto moderationV = 0.009;

	m_hAngle = m_hAngle + p->x * moderationH;
	m_vAngle = m_vAngle + p->y * moderationV;

	auto world = GetConstBufferData()->World;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixRotationY(m_hAngle)));
}

void Camera::InitializeConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	D3D11_SUBRESOURCE_DATA constantBufferData;
	constantBufferData.pSysMem = GetConstBufferData();
	constantBufferData.SysMemPitch = 0;
	constantBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* constBuffer;
	auto result = m_pDeviceResources->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &constBuffer);
	m_pConstBuffer = shared_ptr<ID3D11Buffer>(constBuffer);
}