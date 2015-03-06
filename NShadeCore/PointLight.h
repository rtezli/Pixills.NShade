#pragma once

#include "common.h"
#include "light.h"

class PointLight : public Light
{
public:
	PointLight(DeviceResources* pDeviceResources, XMFLOAT3* position, XMFLOAT3* color, FLOAT* intensity);
	~PointLight();
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4X4 refection;
		XMFLOAT3   camera;
		FLOAT	   time;
	};
	ID3D11Buffer*			const GetConstBuffer(){ return m_pConstBuffer.get(); };
	ConstantBufferData*		const GetConstBufferData(){ return m_pConstBufferData.get(); };
private:
	void							InitializeConstantBuffer();

	DeviceResources*				m_pDeviceResources;
	shared_ptr<XMFLOAT3>			m_pPosition;
	shared_ptr<XMFLOAT3>			m_pColor;
	shared_ptr<FLOAT>				m_pIntensity;
	shared_ptr<ID3D11Buffer>		m_pConstBuffer;
	shared_ptr<ConstantBufferData>	m_pConstBufferData;
};

