#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = resources;
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	m_pWorldMatrix		= new XMFLOAT4X4();
	m_pViewMatrix		= new XMFLOAT4X4();
	m_pProjectionMatrix = new XMFLOAT4X4();

	m_radius = 6.0f;
	m_hAngle = 0.0f;
	m_vAngle = 0.0f;

	auto z = m_radius * sin(m_hAngle * -1);
	auto x = sqrt(pow(m_radius, 2) - pow(z, 2));

	m_eyePosition	= new XMFLOAT3{ 0.0f, 4.0f, m_radius };
	m_focusPosition = new XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_upDirection	= new XMFLOAT3{ 0.0f, 1.0f, 0.0f };

	auto wMatrix = XMMatrixTranspose(XMMatrixIdentity());
	XMStoreFloat4x4(m_pWorldMatrix, wMatrix);

	auto vMatrix = XMMatrixTranspose(
		XMMatrixLookAtRH(
			XMLoadFloat3(m_eyePosition), 
			XMLoadFloat3(m_focusPosition), 
			XMLoadFloat3(m_upDirection)
		)
	);

	XMStoreFloat4x4(m_pViewMatrix, vMatrix);

	float fovAngleY = GetFieldOfView();
	float aspectRatio = GetAspectRatio();

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 1.0f;
	}

	XMMATRIX	perspectiveMatrix	= XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.1f, 100.0f);
	XMFLOAT4X4	orientation			= ScreenRotation::Rotation0;
	XMMATRIX	orientationMatrix	= XMLoadFloat4x4(&orientation);
   
	XMStoreFloat4x4(m_pProjectionMatrix, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	Update();
}

void Camera::Move(POINT* p)
{
	XMStoreFloat4x4(m_pWorldMatrix, XMMatrixTranspose(XMMatrixTranslation(p->x, 0.0, p->y)));
	Update();
}

void Camera::Rotate(POINT* p)
{
	auto moderationH = 0.001;
	auto moderationV = 0.009;

	m_hAngle = m_hAngle + p->x * moderationH;
	m_vAngle = m_vAngle + p->y * moderationV;

	XMStoreFloat4x4(m_pWorldMatrix, XMMatrixTranspose(XMMatrixRotationY(m_hAngle)));
	m_pDeviceResources->CameraConstBufferData->world = *m_pWorldMatrix;
}

void Camera::Update()
{	
	XMFLOAT4X4 reflect;
	ConstantBufferData constBuffer = { *m_pWorldMatrix, *m_pViewMatrix, *m_pProjectionMatrix, reflect, *m_eyePosition, 0 };
	m_pDeviceResources->CameraConstBufferData = new ConstantBufferData(constBuffer);
}

HRESULT Camera::InitializeConstantBuffer()
{
	// Belongs to renderer class
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
	constantBufferData.pSysMem = &m_pDeviceResources->CameraConstBufferData;
	constantBufferData.SysMemPitch = 0;
	constantBufferData.SysMemSlicePitch = 0;

	return m_pDeviceResources->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &m_pDeviceResources->CameraConstBufferData);
}