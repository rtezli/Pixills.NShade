#pragma once

#include "windows.h"
#include "d3d11.h"
#include "dxgi.h"
#include "directxmath.h"

#include "stdafx.h"
#include "export.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"

using namespace DirectX;

EXTERN class API D3DSystem
{
private :
	enum Perspective
	{
		FIRST_PERSON,
		THIRD_PERSON,
		ISOMETRIC
	};

public:
	D3DSystem();
	~D3DSystem();
	HRESULT Initialize(int screenWidth, int screenHeight, bool vsync, HWND* hwnd, bool fullscreen, float screenDepth, float screenNear);
	HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	HRESULT InitializeWindow(int& screenWidth, int& screenHeight);
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, UINT16 viewAngle);
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength);
	VOID	Render();
	VOID	Destroy();
private:
	HRESULT CreateDevice();
	HRESULT	CreateSwapChain();
	HRESULT CreateCamera();
	HWND	Create3DWindow(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
	HRESULT	LoadModels();
	HRESULT	ApplyShaders();
	HRESULT	CreateRenderer();
private:
	Renderer*				m_pRenderer = 0;
	Camera*					m_pCamera = 0;
	Model*					m_pModel = 0;

	IDXGIDevice*			m_pDXGIDevice = 0;
	IDXGIAdapter*			m_pDXGIAdapter = 0;
	IDXGIFactory1*			m_pDXGIFactory = 0;
	IDXGISwapChain*			m_pSwapChain = 0;

	ID3D11Device*			m_pDevice = 0;
	ID3D11DeviceContext*	m_pDeviceContext = 0;
	ID3D11RenderTargetView*	m_pRenderTarget = 0;
	ID3D11DepthStencilView* m_pDepthStencilView = 0;
	HWND*					m_pWindow = 0;

	D3D_FEATURE_LEVEL		m_D3dFeatureLevel;
	XMMATRIX				m_WorldMatrix;
	XMMATRIX				m_ViewMatrix;
	XMMATRIX				m_ProjectionMatrix;

	XMVECTOR				m_Position;
	XMVECTOR				m_Rotation;

	int						m_ScreenWidth;
	int						m_ScreenHeight;
	BOOL					m_vsync_enabled;
	BOOL					m_Fullscreen;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static D3DSystem* ApplicationHandle = 0;