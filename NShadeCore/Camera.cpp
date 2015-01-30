#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{

}


Camera::~Camera()
{
}


void Camera::Build(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	viewMatrix = XMMatrixPerspectiveRH(ViewWidth, ViewHeight, NearZ, FarZ);
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