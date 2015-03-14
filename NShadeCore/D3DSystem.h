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
    HRESULT InitializeForWindow(BOOL vsync, HINSTANCE* windowInstance, HWND* windowHandle, BOOL fullscreen);
    HRESULT InitializeWithWindow(INT screenWidth, INT screenHeight, BOOL vsync, BOOL fullscreen);
    LRESULT MessageHandler(HWND* hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
    HRESULT Initialize();
private:
    HRESULT                 InitializeWindow(INT screenWidth, INT screenHeight);
    HRESULT                 CreateDevice();
    HRESULT                 CreateInput();
    HRESULT                 GetRenderQualitySettings(ID3D11Device* device);
    vector<MSAA>*           ProduceMsaaCapability(vector<MSAA>* msaaOptions, INT i);
    HRESULT                 CreateCamera();
    HRESULT                 LoadModels();
    HRESULT                 CreateRenderer();
    HRESULT                 AttachOnRotate();
    HRESULT                 DetachOnRotate();
    VOID                    CreateViewPort();
    VOID                    Render();
private:
    POINT                   *_lastPointerPosition;
    shared_ptr<Renderer>    _renderer;
    shared_ptr<Camera>      _camera;
    shared_ptr<Input>       _inputDevices;
    shared_ptr<Model>       _model;

    D3D_FEATURE_LEVEL       _d3dFeatureLevel;

    XMVECTOR                _position;
    XMVECTOR                _rotation;

    FLOAT                   _viewportWidth;
    FLOAT                   _viewportHeight;
    FLOAT                   _nearZ;
    FLOAT                   _farZ;
    BOOL                    _trackInput;
};
