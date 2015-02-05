#include "stdafx.h"
#include "camera.h"

Camera::Camera(float ViewWidth, float viewHeight, float NearZ, float FarZ)
{
	Initialize(ViewWidth, viewHeight, NearZ, FarZ);
}


Camera::~Camera()
{
}


void Camera::Initialize(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	float aspectRatio = 800 / 600;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	auto orientation = m_deviceResources->GetOrientationTransform3D();
	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fovAngleY,aspectRatio,0.01f,100.0f);

	auto orientationMatrix = XMLoadFloat4x4(orientation);
	DirectX::XMStoreFloat4x4(&m_constBuffer->projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	static const DirectX::XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&m_constBuffer->view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up)));

	auto matrix = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);
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