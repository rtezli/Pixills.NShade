#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = resources;
	m_eyePosition = new XMVECTORF32{ 1.0f, 1.0f, -3.0f, 0.0f };
	m_focusPosition = new XMVECTORF32{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_upDirection = new XMVECTORF32{ 0.0f, 1.0f, 0.0f, 0.0f };
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	m_pDeviceResources->ConstBufferData = new ConstantBufferData();
	m_pDeviceResources->ConstBufferData->world = GetWorldMatrix();
	m_pDeviceResources->ConstBufferData->view = GetViewMatrix();
	m_pDeviceResources->ConstBufferData->projection = GetProjectionMatrix();
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
	auto mv = (0,1) * p->x;
	auto radius = sqrt(pow(p->x, 2) + pow(p->y, 2));
	auto angle = atan(p->y / p->x);
	angle += mv;
	auto x = sin(angle) * radius;
	auto z = sqrt(pow(radius, 2) + pow(p->y, 2));
	m_eyePosition = new XMVECTORF32{ x, 1.0f, z, 0.0f };
}