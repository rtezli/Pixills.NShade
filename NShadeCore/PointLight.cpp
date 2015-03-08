#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight(DeviceResources *pDeviceResources, XMFLOAT3 *pPosition, XMFLOAT3 *pColor, FLOAT *pIntensity)
{
	_deviceResources = pDeviceResources;
	_position = shared_ptr<XMFLOAT3>(pPosition);
	_color = shared_ptr<XMFLOAT3>(pColor);
	_intensity = shared_ptr<FLOAT>(pIntensity);
}

VOID PointLight::InitializeConstantBuffer()
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
	auto result = _deviceResources->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &constBuffer);
	_constBuffer = shared_ptr<ID3D11Buffer>(constBuffer);
}

VOID PointLight::CreateResources()
{
	//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	//shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	//ID3D11Resource *resource;
	//ID3D11ShaderResourceView *resourceView;
	//auto result = _deviceResources->Device->CreateShaderResourceView(resource, &shaderResourceViewDesc, &resourceView);
	//_resourceView = shared_ptr<ID3D11ShaderResourceView>(resourceView);
}

XMFLOAT4* PointLight::GetPositionIntensity()
{
	return new XMFLOAT4(_position->x, _position->y, _position->x, _intensity);
}