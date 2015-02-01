#include "stdafx.h"
#include "camera.h"

Camera::Camera()
{

}


Camera::~Camera()
{
}


void Camera::Build(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	//auto matrix = DirectX::XMMatrixPerspectiveLH(ViewWidth, ViewHeight, NearZ, FarZ);
	//DirectX::XMStoreFloat4x4(&m_viewMatrix, matrix);
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