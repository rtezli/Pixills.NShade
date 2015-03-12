#pragma once

#include "common.h"
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
	void				Update();
	FLOAT				GetAspectRatio(){ return _deviceResources->ViewPort->Width / _deviceResources->ViewPort->Height; };
	FLOAT				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	XMFLOAT4X4*			m_pWorldMatrix;
	XMFLOAT4X4*			m_pViewMatrix;
	XMFLOAT4X4*			m_pProjectionMatrix;
	DeviceResources*	_deviceResources;
	XMFLOAT3*			m_eyePosition;
	XMFLOAT3*			m_focusPosition;
	XMFLOAT3*			m_upDirection;
	FLOAT				m_hAngle;
	FLOAT				m_vAngle;
	FLOAT				m_radius;
};

