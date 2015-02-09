#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = resources;

	XMVECTORF32 eye = { 0.0f, 5.0f, 20.0, 0.0f };
	XMVECTORF32 at = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_eyePosition = eye;
	m_focusPosition = at;
	m_upDirection = up;
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	float aspectRatio = GetAspectRatio();
	float fieldOfView = GetFieldOfView();

	m_pDeviceResources->ConstBufferData = new ConstantBufferData();

	m_pDeviceResources->ConstBufferData->view = GetViewMatrix();
	m_pDeviceResources->ConstBufferData->projection = GetProjectionMatrix();
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	XMFLOAT4X4 view;
	auto matrix = XMMatrixTranspose(XMMatrixLookAtRH(m_eyePosition, m_focusPosition, m_upDirection));
	XMStoreFloat4x4(&view, matrix);
	return view;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	XMFLOAT4X4 projection;
	auto matrix = XMMatrixTranspose(XMMatrixPerspectiveFovRH(GetFieldOfView(), GetAspectRatio(), m_pDeviceResources->NearZ, m_pDeviceResources->FarZ));
	XMStoreFloat4x4(&projection, matrix);
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