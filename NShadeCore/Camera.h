#pragma once

#include "includes.h"
#include "deviceresources.h"

class Camera
{
public:
	Camera(DeviceResources* resources, float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	~Camera();
public:
	void				Initialize(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	void				RotateHorizontal(float angle);
	void				RotateVertical(float angle);
	void				MoveX(float angle);
	void				MoveY(float angle);
	void				MoveZ(float angle);
private:
	DeviceResources*							DeviceResource(){ return m_pDeviceResources.get(); }
	shared_ptr<XMFLOAT4X4>						m_position;
	shared_ptr<XMFLOAT4X4>						m_direction;
	shared_ptr<DeviceResources>					m_pDeviceResources;
};

