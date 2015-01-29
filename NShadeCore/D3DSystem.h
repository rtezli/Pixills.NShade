#pragma once

#include "Export.h"
#include "windows.h"
#include "d3d11.h"
#include "directxmath.h"
#include "renderer.h"
#include "camera.h"

using namespace DirectX;

namespace NShade{
	NSHADECORE_API enum Perspective
	{
		FIRST_PERSON,
		THIRD_PERSON,
		ISOMETRIC
	}Perspective;

	class NSHADECORE_API D3DSystem
	{
	public:
		D3DSystem();
		~D3DSystem();
		HRESULT Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
		HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
		HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, UINT16 viewAngle);
		HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength);
	private:
		HRESULT D3DSystem::CreateDevice();
		HRESULT D3DSystem::CreateSwapChain();
		HRESULT D3DSystem::CreateCamera();
		HRESULT D3DSystem::LoadModels();
		HRESULT D3DSystem::ApplyShaders();
		HRESULT D3DSystem::CreateRenderer();
		HRESULT D3DSystem::Render();
	private:
		D3D_FEATURE_LEVEL m_d3dFeatureLevel;
		Renderer*	m_renderer;
		Camera*		m_camera;

	    XMMATRIX	m_worldMatrix;
		XMMATRIX	m_viewMatrix;
		XMMATRIX	m_projectionMatrix;

		XMVECTOR	m_position;
		XMVECTOR	m_rotation;
	};
}

