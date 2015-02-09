#include "stdafx.h"
#include "DeviceResources.h"

DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
	Quality = 0;
	SamplesCount = 1;
	BufferCount = 2;
	SwapChainFlags = 0;
	DefaultColor = Colors::LightGray;
	Dpi = 96.00;
}

DeviceResources::~DeviceResources()
{

}
