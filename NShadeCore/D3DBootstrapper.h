#pragma once

#include "Export.h"
#include "windows.h"
#include "d3d11.h"
#include "directxmath.h"

using namespace DirectX;

namespace NShade{
	class NSHADECORE_API D3DBootstrapper
	{
	public:
		D3DBootstrapper();
		~D3DBootstrapper();
		static HRESULT Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

	private:
		HRESULT D3DBootstrapper::GetDeviceCaps();
		HRESULT D3DBootstrapper::CreateDevice();
		HRESULT D3DBootstrapper::CreateSwapChain();
	private:
	    XMMATRIX m_worldMatrix;
		XMMATRIX  m_viewMatrix;
		XMMATRIX  m_projectionMatrix;

		XMVECTOR  m_position;
		XMVECTOR  m_rotation;
	};
}

