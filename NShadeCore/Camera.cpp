#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = resources;

	static const XMVECTORF32 eye =	{ 0.0f, 5.0f, 20.0, 0.0f };
	static const XMVECTORF32 at =	{ 0.0f, 0.0f,  0.0f, 0.0f };
	static const XMVECTORF32 up =	{ 0.0f, 1.0f,  0.0f, 0.0f };

	EyePosition = eye;
	FocusPosition = at;
	UpDirection = up;
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	float aspectRatio = GetAspectRatio();
	float fieldOfView = GetFieldOfView();

	m_pDeviceResources->ConstBufferData = new ConstantBufferData();

	XMStoreFloat4x4(&m_pDeviceResources->ConstBufferData->view, GetViewMatrix());
	XMStoreFloat4x4(&m_pDeviceResources->ConstBufferData->projection, GetProjectionMatrix());
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