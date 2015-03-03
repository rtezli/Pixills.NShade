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
	void				Move(POINT* p);
	void				Rotate(POINT* p);
	HRESULT				InitializeConstantBuffer();
	void				Update();
	float				GetAspectRatio(){ return m_pDeviceResources->ViewPort->Width / m_pDeviceResources->ViewPort->Height; };
	float				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	XMFLOAT4X4*			m_pWorldMatrix;
	XMFLOAT4X4*			m_pViewMatrix;
	XMFLOAT4X4*			m_pProjectionMatrix;
	DeviceResources*	m_pDeviceResources;
	XMFLOAT3*			m_eyePosition;
	XMFLOAT3*			m_focusPosition;
	XMFLOAT3*			m_upDirection;
	float				m_hAngle;
	float				m_vAngle;
	float				m_radius;
};

