#pragma once

#include "common.h"

enum Perspective : char
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};

class Camera
{
public:
	Camera(DeviceResources* resources);
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 World;
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
		XMFLOAT4   CameraPosition;
	};

public:
	ID3D11Buffer*		const GetConstBuffer(){ return m_pConstBuffer.get(); };
	ConstantBufferData*	const GetConstBufferData(){ return m_pConstBufferData.get(); };

	VOID				Initialize();
	VOID				InitializeConstantBuffer();

	VOID				Move(POINT* p);
	VOID				Rotate(POINT* p);
	VOID				SetPosition(XMFLOAT3* p);
	VOID				SetFocusPoint(XMFLOAT3* p);

	FLOAT				const GetVerticalAngle(){ return m_vAngle; };
	FLOAT				const GetHorizontalAngle(){ return m_hAngle; };
	FLOAT				const GetAspectRatio(){ return m_pDeviceResources->ViewPort->Width / m_pDeviceResources->ViewPort->Height; };
	FLOAT				const GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	shared_ptr<XMFLOAT3>			m_eyePosition;
	shared_ptr<XMFLOAT3>			m_focusPosition;
	shared_ptr<XMFLOAT3>			m_upDirection;
	
	shared_ptr<ID3D11Buffer>		m_pConstBuffer;
	shared_ptr<ConstantBufferData>	m_pConstBufferData;

	DeviceResources*				m_pDeviceResources;

	FLOAT							m_hAngle;
	FLOAT							m_vAngle;
	FLOAT							m_radius;
};

