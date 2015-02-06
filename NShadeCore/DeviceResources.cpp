#include "stdafx.h"
#include "DeviceResources.h"


DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
}


DeviceResources::~DeviceResources()
{
}

DirectX::XMFLOAT4X4* DeviceResources::GetOrientationTransform3D()
{
	return new DirectX::XMFLOAT4X4();
}
