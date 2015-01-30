#pragma once

#include "Export.h"
#include "windows.h"
#include "d3d11.h"
#include "dxgi.h"
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
		HRESULT CreateDevice();
		HRESULT	CreateSwapChain();
		HRESULT CreateCamera();
		HWND	Create3DWindow(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
		HRESULT	LoadModels();
		HRESULT	ApplyShaders();
		HRESULT	CreateRenderer();
		HRESULT	Render();
	private:
		ID3D11Device*			m_pDevice;
		ID3D11DeviceContext*	m_pDeviceContext;
		IDXGIFactory1*			m_pDXGIFactory;
		IDXGISwapChain*			m_pSwapChain;
		ID3D11RenderTargetView*	m_pRenderTarget;
		ID3D11DepthStencilView* m_pDepthStencilView;
		D3D_FEATURE_LEVEL		m_D3dFeatureLevel;
		Renderer*				m_pRenderer;
		Camera*					m_pCamera;

		XMMATRIX				m_WorldMatrix;
		XMMATRIX				m_ViewMatrix;
		XMMATRIX				m_ProjectionMatrix;

		XMVECTOR				m_Position;
		XMVECTOR				m_Rotation;
	};
}

