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
	RenderingQuality quality0 = {  0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, false }; // Should work for everyone
	RenderingQuality quality2 = {  2, 2, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality4 = { 16, 4, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality8 = { 32, 8, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true }; // Works on my machine ;)

	RenderingQuality quality42 = { 16, 4, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_D32_FLOAT, true };

	RenderQuality = new RenderingQuality(quality4);
}

DeviceResources::~DeviceResources()
{

}
