#pragma once

#include "includes.h"
#include "deviceresources.h"

class Camera
{
public:
	Camera(DeviceResources* resources);
	~Camera();
public:
	ID3D11Buffer*		const GetConstBuffer(){ return m_pCameraConstBuffer.get(); };
	ConstantBufferData*	const GetConstBufferData(){return m_pCameraConstBufferData.get(); };

	void				Initialize();
	void				Move(POINT* p);
	void				Rotate(POINT* p);
	HRESULT				InitializeConstantBuffer();
	void				Update();
	float				GetAspectRatio(){ return m_pDeviceResources->ViewPort->Width / m_pDeviceResources->ViewPort->Height; };
	float				GetFieldOfView(){ return 70.0f * XM_PI / 180.0f; };
private:
	XMFLOAT4X4			m_pWorldMatrix;
	XMFLOAT4X4			m_pViewMatrix;
	XMFLOAT4X4			m_pProjectionMatrix;
	XMFLOAT3			m_eyePosition;
	XMFLOAT3			m_focusPosition;
	XMFLOAT3			m_upDirection;
	
	shared_ptr<ID3D11Buffer>		m_pCameraConstBuffer;
	shared_ptr<ConstantBufferData>	m_pCameraConstBufferData;

	DeviceResources*				m_pDeviceResources;

	float							m_hAngle;
	float							m_vAngle;
	float							m_radius;
};

