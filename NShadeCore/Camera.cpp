#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources)
{
	m_pDeviceResources = resources;

	static const XMVECTORF32 eye = { 0.0f, 0.7f, -20.0, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };

	m_position = eye;
	m_targetPoint = at;
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	float aspectRatio = m_pDeviceResources->ScreenWidth / m_pDeviceResources->ScreenHeight;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	
	m_pDeviceResources->ConstBufferData = new ConstantBufferData();

	auto perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, m_pDeviceResources->NearZ, m_pDeviceResources->FarZ);
	auto orientationMatrix = XMLoadFloat4x4(&ScreenRotation::Rotation0);
	auto projection = XMMatrixTranspose(perspectiveMatrix * orientationMatrix);

	XMStoreFloat4x4(&m_pDeviceResources->ConstBufferData->projection, perspectiveMatrix);


	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_pDeviceResources->ConstBufferData->view, XMMatrixTranspose(XMMatrixLookAtLH(m_position, m_targetPoint, up)));
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