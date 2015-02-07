#include "stdafx.h"
#include "camera.h"

Camera::Camera(DeviceResources* resources, float viewWidth, float ViewHeight, float NearZ, float FarZ)
{
	m_pDeviceResources = shared_ptr<DeviceResources>(resources);
	Initialize(viewWidth, ViewHeight, NearZ, FarZ);
}

Camera::~Camera()
{
}

void Camera::Initialize(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	float aspectRatio = 800 / 600;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	auto res = DeviceResource();

	res->ConstantBufferData = new MVPConstantBuffer();

	auto orientation = res->GetOrientationTransform3D;
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);

	auto orientationMatrix = XMLoadFloat4x4(orientation);
	XMStoreFloat4x4(&res->ConstantBufferData->projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	static const DirectX::XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

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