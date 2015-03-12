#pragma once

#include "common.h"
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
	HRESULT InitializeForWindow(BOOL vsync, HINSTANCE*, HWND* window, BOOL fullscreen);
	HRESULT InitializeWithWindow(INT screenWidth, INT screenHeight, BOOL vsync, BOOL fullscreen);
	LRESULT MessageHandler(HWND* hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	HRESULT Initialize();
	void	Render();
private:
	HRESULT					InitializeWindow(INT screenWidth, INT screenHeight);
	HRESULT					CreateDevice();
	HRESULT					CreateInput();
	HRESULT					GetRenderQualitySettings(ID3D11Device* device);
	vector<MSAA>* 			ProduceMsaaCapability(vector<MSAA>* msaaOptions, INT i);
	HRESULT					CreateCamera();
	D3D11_VIEWPORT*			CreateViewPort(HWND* hwnd);
	HRESULT					LoadModels();
	HRESULT					CreateRenderer();
	HRESULT					AttachOnRotate();
	HRESULT					DetachOnRotate();
private:
	DeviceResources*						_deviceResources;
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

	FLOAT									m_viewportWidth;
	FLOAT									m_viewportHeight;
	FLOAT									m_nearZ;
	FLOAT									m_farZ;

	BOOL									m_fullScreen;
	BOOL									m_vSync;
	BOOL									m_trackInput;
};
