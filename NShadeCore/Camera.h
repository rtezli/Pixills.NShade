#pragma once

#include "common.h"
#include "deviceresources.h"

class Camera
{
public:
	Camera(DeviceResources *resources);
	~Camera();
public:
	void				Initialize();
	void				Move(POINT* p);
	void				Rotate(POINT* p);
	void				Update();
	FLOAT				GetAspectRatio(){ return _deviceResources->ViewPort->Width / _deviceResources->ViewPort->Height; };
	FLOAT				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	XMFLOAT4X4*			_worldMatrix;
	XMFLOAT4X4*			_viewMatrix;
	XMFLOAT4X4*			_projectionMatrix;
	DeviceResources*	_deviceResources;
	XMFLOAT3*			_eyePosition;
	XMFLOAT3*			_focusPosition;
	XMFLOAT3*			_upDirection;
	FLOAT				_hAngle;
	FLOAT				_vAngle;
	FLOAT				_radius;
};

