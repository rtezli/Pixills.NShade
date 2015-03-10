#include "stdafx.h"
#include "light.h"

Light::Light(DeviceResources *deviceResources)
{
	_deviceResources = deviceResources;
}

VOID Light::InitializeBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(XMFLOAT4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = Parameters();
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* buffer;
	auto result = _deviceResources->Device->CreateBuffer(&bufferDesc, &bufferData, &buffer);
	_buffer = shared_ptr<ID3D11Buffer>(buffer);
}