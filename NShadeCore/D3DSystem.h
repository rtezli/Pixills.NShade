#pragma once
#include "export.h"
#include "d3d11.h"
#include "dxgi.h"
//#include "directxmath.h"

#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "input.h"

EXTERN class API D3DSystem
{
public:
	D3DSystem();
	~D3DSystem();
public:
	HRESULT InitializeForWindow(BOOL vsync, HWND* hwnd, BOOL fullscreen, FLOAT screenDepth, FLOAT screenNear);
	HRESULT InitializeWithWindow(INT32 screenWidth, INT32 screenHeight, BOOL vsync, BOOL fullscreen, FLOAT screenDepth, FLOAT screenNear);
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, UINT16 viewAngle);
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength);
	VOID	Render();
	VOID	Destroy();
private:
	HRESULT CreateDevice();
	HRESULT	CreateSwapChain();
	HRESULT CreateCamera();
	HRESULT InitializeWindow(int screenWidth, int screenHeight);
	HWND	Create3DWindow(INT32 screenWidth, INT32 screenHeight, BOOL vsync, HWND* hwnd, BOOL fullscreen);
	HRESULT	LoadModels();
	HRESULT	ApplyShaders();
	HRESULT	CreateRenderer();
private:
	std::shared_ptr<Renderer>				m_pRenderer;
	std::shared_ptr<Camera>					m_pCamera;
	std::shared_ptr<Input>					m_pInputDevices;
	std::shared_ptr<Model>					m_pModel;

	std::shared_ptr<IDXGIDevice>			m_pDXGIDevice;
	std::shared_ptr<IDXGIAdapter>			m_pDXGIAdapter;
	std::shared_ptr<IDXGIFactory1>			m_pDXGIFactory;
	std::shared_ptr<IDXGISwapChain>			m_pSwapChain;
	std::shared_ptr<ID3D11Texture2D>		m_pRenderBuffer;

	std::shared_ptr<ID3D11Device>			m_pDevice;
	std::shared_ptr<ID3D11DeviceContext>	m_pDeviceContext;
	std::shared_ptr<ID3D11RenderTargetView>	m_pRenderTarget;
	std::shared_ptr<ID3D11DepthStencilView>	m_pDepthStencilView;
	std::shared_ptr<HWND>					m_pWindow;

	D3D_FEATURE_LEVEL						m_D3dFeatureLevel;
	XMFLOAT4X4								m_WorldMatrix;
	XMFLOAT4X4								m_ViewMatrix;
	XMFLOAT4X4								m_ProjectionMatrix;

	XMVECTOR								m_Position;
	XMVECTOR								m_Rotation;

	INT32									m_ScreenWidth;
	INT32									m_ScreenHeight;
	BOOL									m_vsync_enabled;
	BOOL									m_Fullscreen;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static D3DSystem* ApplicationHandle = 0;

enum Perspective : byte
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};