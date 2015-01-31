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

class D3DSystem
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
	HRESULT Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	HRESULT InitializeWindow(int& screenWidth, int& screenHeight);
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
	VOID	Render();
	VOID	Destroy();

private:
	Renderer*				m_pRenderer;
	Camera*					m_pCamera;
	Model*					m_pModel;
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	IDXGIFactory1*			m_pDXGIFactory;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTarget;
	ID3D11DepthStencilView* m_pDepthStencilView;

	D3D_FEATURE_LEVEL		m_D3dFeatureLevel;
	XMMATRIX				m_WorldMatrix;
	XMMATRIX				m_ViewMatrix;
	XMMATRIX				m_ProjectionMatrix;

	XMVECTOR				m_Position;
	XMVECTOR				m_Rotation;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static D3DSystem* ApplicationHandle = 0;