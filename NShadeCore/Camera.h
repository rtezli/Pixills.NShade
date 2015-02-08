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
private:
	DeviceResources*	m_pDeviceResources;
	XMVECTORF32			m_position;
	XMVECTORF32			m_targetPoint;
};

