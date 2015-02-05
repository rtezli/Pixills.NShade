#pragma once

#include "includes.h"
#include "export.h"

#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "window.h"
#include "input.h"

EXTERN class API D3DSystem
{
public:
	D3DSystem();
	~D3DSystem();
public:
	HRESULT InitializeForWindow(bool vsync, std::shared_ptr<HWND> handle, bool fullscreen, float screenDepth, float screenNear);
	HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	HRESULT Initialize();
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, unsigned short viewAngle);
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, float focalLength);
	void	Render();
private:
	std::shared_ptr<HWND>	InitializeWindow(int screenWidth, int screenHeight);
	HRESULT					CreateDevice();
	HRESULT					CreateCamera();
	HRESULT					LoadModels();
	HRESULT					CreateRenderer();
private:
	std::shared_ptr<Renderer>				m_pRenderer;
	std::shared_ptr<Camera>					m_pCamera;
	std::shared_ptr<Input>					m_pInputDevices;
	std::shared_ptr<Model>					m_pModel;
	std::shared_ptr<Window>					m_pWindow;

	std::shared_ptr<ID3D11Device>			m_pDevice;
	std::shared_ptr<ID3D11DeviceContext>	m_pDeviceContext;
	std::shared_ptr<HWND>					m_pWindowHandle;

	D3D_FEATURE_LEVEL						m_D3dFeatureLevel;

	XMVECTOR								m_Position;
	XMVECTOR								m_Rotation;

	int										m_viewportWidth;
	int										m_viewportHeight;
	int										m_nearZ;
	int										m_farZ;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static D3DSystem* ApplicationHandle = 0;

enum Perspective : char
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};