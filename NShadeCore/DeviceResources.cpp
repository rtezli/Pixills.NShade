#include "stdafx.h"
#include "DeviceResources.h"

DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
	Shaders = new ShaderSet();
	SamplesCount = 4;
}

DeviceResources::~DeviceResources()
{

}
