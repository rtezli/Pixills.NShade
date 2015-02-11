#include "stdafx.h"
#include "DeviceResources.h"

DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
	BufferCount = 2;
	SwapChainFlags = 0;
	DefaultColor = Colors::LightGray;
	Dpi = 96.00;

	// TODO : Get available formats dynamically
	RenderingQuality quality0 = { 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM }; // Should work for everyone
	RenderingQuality quality1 = { 0, 1, DXGI_FORMAT_D24_UNORM_S8_UINT };
	RenderingQuality quality2 = { 0, 2, DXGI_FORMAT_D24_UNORM_S8_UINT };
	RenderingQuality quality3 = { 0, 1, DXGI_FORMAT_B8G8R8X8_UNORM_SRGB }; // Should work for everyone
	RenderingQuality quality4 = { 8, 33, DXGI_FORMAT_B8G8R8A8_UNORM };

	RenderQuality = new RenderingQuality(quality4);
}

DeviceResources::~DeviceResources()
{

}
