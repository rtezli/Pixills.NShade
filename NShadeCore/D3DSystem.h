#pragma once

#include "common.h"
#include "export.h"

#include "renderer.h"

using namespace std::chrono;

EXTERN class API D3DSystem
{
public:
	~D3DSystem();
public:
	HRESULT InitializeForWindow(BOOL vsync, HINSTANCE* instance, HWND* handle, BOOL fullscreen);
	HRESULT InitializeWithWindow(INT screenWidth, INT screenHeight, BOOL vsync, BOOL fullscreen);
	LRESULT MessageHandler(HWND *handle, UINT message, WPARAM wParam, LPARAM lParam);

	HRESULT Initialize();
	VOID	Render();
private:
	HRESULT					InitializeWindow(INT screenWidth, INT screenHeight);
	HRESULT					CreateDevice();
	HRESULT					GetRenderQualitySettings(ID3D11Device *device);
	vector<MSAA>* 			ProduceMsaaCapability(vector<MSAA> *msaaOptions, INT i);

	D3D11_VIEWPORT*			CreateViewPort(HWND *hwnd);

	HRESULT					CreateRenderer();
	HRESULT					AttachOnRotate();
	HRESULT					DetachOnRotate();
private:
	DeviceResources*						_deviceResources;
	HINSTANCE*								_windowInstance;
	HWND*									_windowHandle;
	POINT*									_lastPointerPosition;
	shared_ptr<Renderer>					_renderer;

	D3D_FEATURE_LEVEL						_d3dFeatureLevel;

	FLOAT									_viewportWidth;
	FLOAT									_viewportHeight;
	FLOAT									_nearZ;
	FLOAT									_farZ;

	BOOL									_fullScreen;
	BOOL									_vSync;
	BOOL									_trackInput;
};
