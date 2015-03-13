#include "stdafx.h"
#include "deviceresources.h"

DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
}