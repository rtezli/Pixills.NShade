#pragma once

#include "common.h"
#include "deviceresources.h"

EXTERN class API Camera
{
public:
	Camera(DeviceResources *resources);
public:
	void				Initialize();
	void				Move(POINT* p);
	void				Rotate(POINT* p);
	void				Update();
	FLOAT				GetAspectRatio(){ return _resources->ViewPort->Width / _resources->ViewPort->Height; };
	FLOAT				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	DeviceResources		*_resources;

	XMFLOAT4X4*			_worldMatrix;
	XMFLOAT4X4*			_viewMatrix;
	XMFLOAT4X4*			_projectionMatrix;

	XMFLOAT3*			_eyePosition;
	XMFLOAT3*			_focusPosition;
	XMFLOAT3*			_upDirection;
	FLOAT				_hAngle;
	FLOAT				_vAngle;
	FLOAT				_radius;
};

