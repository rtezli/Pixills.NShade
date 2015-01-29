#include "stdafx.h"
#include "Camera.h"

NShade::Camera::Camera()
{

}


NShade::Camera::~Camera()
{
}


void NShade::Camera::Build(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	viewMatrix = XMMatrixPerspectiveRH(ViewWidth, ViewHeight, NearZ, FarZ);
}

void NShade::Camera::RotateHorizontal(float Angle)
{

}

void NShade::Camera::RotateVertical(float Angle)
{

}

void NShade::Camera::MoveX(float Angle)
{

}

void NShade::Camera::MoveY(float Angle)
{

}

void NShade::Camera::MoveZ(float Angle)
{

}