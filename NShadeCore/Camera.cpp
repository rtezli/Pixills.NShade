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
	m_radius = 3;
	m_hAngle = 0;
	m_vAngle = 0;

	auto z = m_radius * sin(m_hAngle * -1);
	auto x = sqrt(pow(m_radius, 2) - pow(z, 2));

	m_eyePosition	= new XMVECTOR{ x, 1.0f, z, 0.0f };
	m_focusPosition = new XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_upDirection	= new XMVECTOR{ 0.0f, 1.0f, 0.0f, 0.0f };

	m_pDeviceResources->ConstBufferData = new ConstantBufferData();
	Update();
}

XMFLOAT4X4 Camera::GetWorldMatrix()
{
	XMFLOAT4X4 view;
	auto matrix = XMMatrixTranspose(XMMatrixIdentity());
	XMStoreFloat4x4(&view, matrix);
	return view;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	Debug::WriteLine(L"CALL : Camera::GetViewMatrix\n");
	XMFLOAT4X4 view;
	auto matrix = XMMatrixTranspose(XMMatrixLookAtRH(*m_eyePosition, *m_focusPosition, *m_upDirection));
	XMStoreFloat4x4(&view, matrix);
	return view;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	Debug::WriteLine(L"CALL : Camera::GetProjectionMatrix\n");

	XMFLOAT4X4 projection;

	float fovAngleY = GetFieldOfView();
	float aspectRatio = GetAspectRatio();

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f);
	XMFLOAT4X4 orientation = ScreenRotation::Rotation0;
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
	//auto matrix = XMMatrixTranspose(XMMatrixPerspectiveFovRH(GetFieldOfView(), GetAspectRatio(), m_pDeviceResources->NearZ, m_pDeviceResources->FarZ));
	//XMStoreFloat4x4(&projection, matrix);
	return projection;
}

void Camera::RotateHorizontal(float Angle)
{

}

void Camera::RotateVertical(float Angle)
{

}

void Camera::Move(POINT* p)
{
	if (p == NULL)
	{
		return;
	}
	if (p->x == 0)
	{
		return;
	}

	auto moderationH = 0.01;
	auto moderationV = 0.09;

	m_hAngle += p->x * moderationH;
	m_vAngle += p->y * moderationV;
	
	//if (m_hAngle >= 360)
	//{
	//	m_hAngle = 0.0 + m_hAngle - 360;
	//}
	//if (m_hAngle <= 0)
	//{
	//	m_hAngle = 360.00 + m_hAngle;
	//}
	
	auto newZ = m_radius * sin(m_hAngle * -1);
	auto newX = sqrt(pow(m_radius, 2) - pow(newZ, 2));

	m_eyePosition = new XMVECTOR{ newX, 1.0f, newZ, 0.0f };

	Update();
}

void Camera::Update()
{
	m_pDeviceResources->ConstBufferData->world = GetWorldMatrix();
	m_pDeviceResources->ConstBufferData->view = GetViewMatrix();
	m_pDeviceResources->ConstBufferData->projection = GetProjectionMatrix();
}