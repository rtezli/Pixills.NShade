#include "stdafx.h"
#include "DeviceResources.h"

DeviceResources::DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Device = device;
	DeviceContext = deviceContext;
	BufferCount = 2;
	SwapChainFlags = 0;
	DefaultColor = Colors::White;
	Dpi = 96.00;

	// TODO : Get available formats dynamically
	// 8 bit per channel (rgba)
	RenderingQuality quality0_8 = { 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, false }; // Should work for everyone
	RenderingQuality quality2_8 = { 2, 2, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality4_8 = { 16, 4, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality8_8 = { 32, 8, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true }; // Works on my machine ;)

	// 16 bit per channel  (rgba)
	RenderingQuality quality0_16 = { 0, 1, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_D32_FLOAT, true };
 
	RenderQuality = new RenderingQuality(quality0_8);
}

DeviceResources::~DeviceResources()
{

}
