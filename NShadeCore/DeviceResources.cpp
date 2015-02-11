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
	RenderingQuality quality0 = { 0,  1, DXGI_FORMAT_B8G8R8A8_UNORM, false }; // Should work for everyone
	RenderingQuality quality1 = { 1,  1, DXGI_FORMAT_B8G8R8A8_UNORM, false };
	RenderingQuality quality2 = { 2,  5, DXGI_FORMAT_B8G8R8A8_UNORM, true };
	RenderingQuality quality3 = { 4, 17, DXGI_FORMAT_B8G8R8A8_UNORM, true };
	RenderingQuality quality4 = { 8, 17, DXGI_FORMAT_B8G8R8A8_UNORM, true };

	RenderingQuality quality5 = { 0, 17, DXGI_FORMAT_D24_UNORM_S8_UINT, true };
	RenderingQuality quality6 = { 1, 4, DXGI_FORMAT_D24_UNORM_S8_UINT, true };
	RenderQuality = new RenderingQuality(quality6);
}

DeviceResources::~DeviceResources()
{

}
