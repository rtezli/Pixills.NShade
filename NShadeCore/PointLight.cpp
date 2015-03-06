#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight(DeviceResources* pDeviceResources, XMFLOAT3* position, XMFLOAT3* color, FLOAT* intensity)
{
	m_pDeviceResources = pDeviceResources;
	m_pPosition = shared_ptr<XMFLOAT3>(position);
	m_pColor = shared_ptr<XMFLOAT3>(color);
	m_pIntensity = shared_ptr<FLOAT>(intensity);
}


PointLight::~PointLight()
{
}

void PointLight::InitializeConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	D3D11_SUBRESOURCE_DATA constantBufferData;
	constantBufferData.pSysMem = GetConstBufferData();
	constantBufferData.SysMemPitch = 0;
	constantBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* constBuffer;
	auto result = m_pDeviceResources->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &constBuffer);
	m_pConstBuffer = shared_ptr<ID3D11Buffer>(constBuffer);
}