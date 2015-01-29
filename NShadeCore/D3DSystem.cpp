#include "stdafx.h"
#include "d3dsystem.h"

NShade::D3DSystem::D3DSystem()
{
}


NShade::D3DSystem::~D3DSystem()
{
}

HRESULT NShade::D3DSystem::Initialize(
	int screenWidth,
	int screenHeight,
	bool vsync,
	HWND hwnd,
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	return 0;
}

HRESULT SetCamera(XMVECTOR position, XMVECTOR direction)
{

}

HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength)
{

}

HRESULT NShade::D3DSystem::GetDeviceCaps()
{
	return 0;
}
	
HRESULT NShade::D3DSystem::CreateDevice()
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateSwapChain()
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateCamera()
{

}

HRESULT NShade::D3DSystem::LoadModels()
{

}

HRESULT NShade::D3DSystem::ApplyShaders()
{

}

HRESULT NShade::D3DSystem::Render()
{
	// TODO : limit FPS to a value
	while (m_isRendering)
	{
		m_renderer->Render();
	}
}
