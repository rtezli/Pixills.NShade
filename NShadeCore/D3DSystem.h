#pragma once

#include "includes.h"
#include "export.h"

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
	HRESULT Initialize();
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, UINT16 viewAngle);
	HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength);
	VOID	Render();
private:
	HRESULT CreateDevice();
	HRESULT CreateCamera();
	HRESULT InitializeWindow(int screenWidth, int screenHeight);
	HWND	Create3DWindow(INT32 screenWidth, INT32 screenHeight, BOOL vsync, HWND* hwnd, BOOL fullscreen);
	HRESULT	LoadModels();
	HRESULT	CreateRenderer();
private:
	std::shared_ptr<Renderer>				m_pRenderer;
	std::shared_ptr<Camera>					m_pCamera;
	std::shared_ptr<Input>					m_pInputDevices;
	std::shared_ptr<Model>					m_pModel;

	std::shared_ptr<ID3D11Device>			m_pDevice;
	std::shared_ptr<ID3D11DeviceContext>	m_pDeviceContext;
	std::shared_ptr<HWND>					m_pWindow;

	D3D_FEATURE_LEVEL						m_D3dFeatureLevel;

	XMVECTOR								m_Position;
	XMVECTOR								m_Rotation;
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