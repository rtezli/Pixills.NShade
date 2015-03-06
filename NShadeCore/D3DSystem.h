#pragma once

#include "common.h"
#include "export.h"

#include "renderer.h"

using namespace std::chrono;

EXTERN class API D3DSystem
{
public:
	D3DSystem();
	~D3DSystem();
public:
	HRESULT InitializeForWindow(bool vsync, HINSTANCE*, HWND* window, bool fullscreen);
	HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen);
	LRESULT MessageHandler(HWND* hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	HRESULT Initialize();
	void	Render();
private:
	HRESULT					InitializeWindow(int screenWidth, int screenHeight);
	HRESULT					CreateDevice();
	HRESULT					GetRenderQualitySettings(ID3D11Device* device);
	vector<MSAA>* 			ProduceMsaaCapability(vector<MSAA>* msaaOptions, int i);

	D3D11_VIEWPORT*			CreateViewPort(HWND* hwnd);

	HRESULT					CreateRenderer();
	HRESULT					AttachOnRotate();
	HRESULT					DetachOnRotate();
private:
	DeviceResources*						m_pDeviceResources;
	HINSTANCE*								m_pHInstance;
	HWND*									m_pWindowHandle;
	POINT*									m_lastPointerPosition;
	shared_ptr<Renderer>					m_pRenderer;

	D3D_FEATURE_LEVEL						m_D3dFeatureLevel;

	FLOAT									m_viewportWidth;
	FLOAT									m_viewportHeight;
	FLOAT									m_nearZ;
	FLOAT									m_farZ;

	BOOL									m_fullScreen;
	BOOL									m_vSync;
	BOOL									m_trackInput;
};
