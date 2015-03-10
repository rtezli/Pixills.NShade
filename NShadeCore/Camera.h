#pragma once

#include "common.h"

enum Perspective : CHAR
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};

class Camera : public ConstantBufferProvider
{
public:
	Camera(DeviceResources *resources);
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 World;
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
		XMFLOAT4   CameraPosition;
	};

public:
	ID3D11Buffer*		const GetConstBuffer(){ return _constBuffer.get(); };
	ConstantBufferData*	const GetConstBufferData(){ return _constBufferData.get(); };

	ID3D11Buffer*		const GetPositionBuffer(){ return _positionBuffer.get(); };
	XMFLOAT3*			const GetPositionBufferData(){ return _eyePosition.get(); };

	VOID				Initialize();
	VOID				InitializeConstantBuffer();
	VOID				InitializePositionBuffer();
	VOID				MoveTo(XMFLOAT3* p);
	VOID				Rotate(XMFLOAT3* p);
	VOID				SetPosition(XMFLOAT3* p);
	VOID				SetFocusPoint(XMFLOAT3* p);

	FLOAT				const GetVerticalAngle(){ return _vAngle; };
	FLOAT				const GetHorizontalAngle(){ return _hAngle; };
	FLOAT				const GetAspectRatio(){ return _deviceResources->ViewPort->Width / _deviceResources->ViewPort->Height; };
	FLOAT				const GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };


private:
	shared_ptr<XMFLOAT3>			_eyePosition;
	shared_ptr<XMFLOAT3>			_focusPosition;
	shared_ptr<XMFLOAT3>			_upDirection;
	
	shared_ptr<ID3D11Buffer>		_constBuffer;
	shared_ptr<ConstantBufferData>	_constBufferData;

	shared_ptr<ID3D11Buffer>		_positionBuffer;

	DeviceResources*				_deviceResources;

	FLOAT							_hAngle;
	FLOAT							_vAngle;
	FLOAT							_radius;
};

