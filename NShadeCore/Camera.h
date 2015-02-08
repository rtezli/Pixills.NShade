#pragma once

#include "includes.h"
#include "deviceresources.h"

class Camera
{
public:
	Camera(DeviceResources* resources);
	~Camera();
public:
	void				Initialize();
	void				RotateHorizontal(float angle);
	void				RotateVertical(float angle);
	void				MoveX(float angle);
	void				MoveY(float angle);
	void				MoveZ(float angle);

	float				GetAspectRatio(){ return m_pDeviceResources->ScreenWidth / m_pDeviceResources->ScreenHeight; };
	float				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };

	XMMATRIX			GetViewMatrix()
	{ 
		return XMMatrixTranspose(XMMatrixLookAtRH(EyePosition, FocusPosition, UpDirection)); 
	}

	XMMATRIX			GetProjectionMatrix()
	{ 
		return XMMatrixTranspose(XMMatrixPerspectiveFovRH(GetFieldOfView(), GetAspectRatio(), m_pDeviceResources->NearZ, m_pDeviceResources->FarZ)); 
	}

	XMVECTORF32			EyePosition;
	XMVECTORF32			FocusPosition;
	XMVECTORF32			UpDirection;
private:
	DeviceResources*	m_pDeviceResources;
};

