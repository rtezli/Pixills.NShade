#pragma once

#include "common.h"
#include "deviceresources.h"

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
	~Camera();
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 World;
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
	};

public:

	ID3D11Buffer*		const GetConstBuffer(){ return m_pCameraConstBuffer.get(); };
	ConstantBufferData*	const GetConstBufferData(){return m_pCameraConstBufferData.get(); };

	void				Initialize();
	void				Move(POINT* p);
	void				Rotate(POINT* p);
	void				InitializeConstantBuffer();
	float				GetAspectRatio(){ return m_pDeviceResources->ViewPort->Width / m_pDeviceResources->ViewPort->Height; };
	float				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	XMFLOAT3*			m_eyePosition;
	XMFLOAT3*			m_focusPosition;
	XMFLOAT3*			m_upDirection;
	
	shared_ptr<ID3D11Buffer>		m_pCameraConstBuffer;
	shared_ptr<ConstantBufferData>	m_pCameraConstBufferData;

	DeviceResources*				m_pDeviceResources;

	float							m_hAngle;
	float							m_vAngle;
	float							m_radius;
};

