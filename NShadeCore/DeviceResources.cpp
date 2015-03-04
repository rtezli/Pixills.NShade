#include "stdafx.h"
#include "DeviceResources.h"

DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
	BufferCount = 2;
	SwapChainFlags = 0;
	Dpi = 96.00;
}

DeviceResources::~DeviceResources()
{

}
