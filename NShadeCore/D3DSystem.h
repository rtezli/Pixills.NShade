#pragma once

#include "includes.h"
#include "export.h"

#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "input.h"

using namespace std::chrono;

EXTERN class API D3DSystem
{
public:
	D3DSystem();
	~D3DSystem();
public:
	HRESULT InitializeForWindow(bool vsync, HINSTANCE*, HWND* handle, bool fullscreen);
	HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen);
	HRESULT Initialize();
	void	Render();

	LRESULT MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	HRESULT					InitializeWindow(int screenWidth, int screenHeight);
	HRESULT					CreateDevice();
	HRESULT					CreateInput();
	HRESULT					GetRenderQualitySettings(ID3D11Device* device);
	vector<MSAA>* 			ProduceMsaaCapability(vector<MSAA>* msaaOptions, int i);
	HRESULT					CreateCamera();
	D3D11_VIEWPORT*			CreateViewPort(HWND* hwnd);
	HRESULT					LoadModels();
	HRESULT					CreateRenderer();
	HRESULT					AttachOnRotate();
	HRESULT					DetachOnRotate();
private:
	DeviceResources*						m_pDeviceResources;
	HINSTANCE*								m_pHInstance;
	HWND*									m_pWindowHandle;
	POINT*									m_lastPointerPosition;
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
	bool									m_trackInput;
};
