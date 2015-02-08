#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = shared_ptr<DeviceResources>(resources);
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	float aspectRatio = DeviceResource()->ScreenWidth / DeviceResource()->ScreenHeight;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	auto res = DeviceResource();

	DeviceResource()->ConstantBufferData = new MVPConstantBuffer();

	auto perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);

	// No orientation for now
	// auto orientationMatrix = XMLoadFloat4x4(&ScreenRotation::Rotation0);
	// auto projection = XMMatrixTranspose(perspectiveMatrix * orientationMatrix);

	XMStoreFloat4x4(&res->ConstantBufferData->projection, perspectiveMatrix);

	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&res->ConstantBufferData->view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

	auto matrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);
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