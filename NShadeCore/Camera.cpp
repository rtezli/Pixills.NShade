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
	XMVECTORF32 eyePosition		= { 0.0f, 0.7f, -5.0f, 0.0f };
	XMVECTORF32 focusPosition	= { 0.0f, 0.0f,  0.0f, 0.0f };
	XMVECTORF32 upDirection		= { 0.0f, 1.0f,  0.0f, 0.0f };

	Debug::WriteLine(L"CALL : Camera::GetViewMatrix\n");
	XMFLOAT4X4 view;
	auto matrix = XMMatrixTranspose(XMMatrixLookAtRH(eyePosition, focusPosition, upDirection));
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

void Camera::MoveX(float Angle)
{

}

void Camera::MoveY(float Angle)
{

}

void Camera::MoveZ(float Angle)
{

}