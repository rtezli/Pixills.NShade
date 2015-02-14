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
	void				Move(POINT* p);
	void				Update();
	float				GetAspectRatio(){ return m_pDeviceResources->ViewPort->Width / m_pDeviceResources->ViewPort->Height; };
	float				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };

	XMFLOAT4X4 GetWorldMatrix();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
private:
	DeviceResources*	m_pDeviceResources;
	XMVECTOR*			m_eyePosition;
	XMVECTOR*			m_focusPosition;
	XMVECTOR*			m_upDirection;
	float				m_hAngle;
	float				m_vAngle;
	float				m_radius;
};

