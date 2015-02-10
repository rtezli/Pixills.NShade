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
	HRESULT InitializeForWindow(bool vsync, HWND* handle, bool fullscreen);
	HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen);
	HRESULT Initialize();
	void	Render();
private:
	HRESULT					InitializeWindow(int screenWidth, int screenHeight);
	HRESULT					CreateDevice();
	HRESULT					CreateRenderQualitySettings(ID3D11Device* device);
	HRESULT					CreateCamera();
	HRESULT					LoadModels();
	HRESULT					CreateRenderer();
private:
	DeviceResources*						m_pDeviceResources;
	HWND*									m_pWindowHandle;

	shared_ptr<Renderer>					m_pRenderer;
	shared_ptr<Camera>						m_pCamera;
	shared_ptr<Input>						m_pInputDevices;
	shared_ptr<Model>						m_pModel;

	D3D_FEATURE_LEVEL						m_D3dFeatureLevel;

	XMVECTOR								m_Position;
	XMVECTOR								m_Rotation;

	float									m_viewportWidth;
	float									m_viewportHeight;
	float									m_nearZ;
	float									m_farZ;
	bool									m_fullScreen;
	bool									m_vSync;
};

static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
static D3DSystem* ApplicationHandle = 0;

enum Perspective : char
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};