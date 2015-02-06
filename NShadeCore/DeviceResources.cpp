#include "stdafx.h"
#include "DeviceResources.h"


DeviceResources::DeviceResources()
{
}


DeviceResources::~DeviceResources()
{
}

DirectX::XMFLOAT4X4* DeviceResources::GetOrientationTransform3D()
{
	return new DirectX::XMFLOAT4X4();
}
