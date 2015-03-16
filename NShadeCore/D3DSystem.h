#pragma once

#ifndef FPS
#define FPS(fps) {std::chrono::milliseconds(1000 / fps)}
#endif

#include "d3d11.h"
#include "res.h"
#include "rx.hpp"
#include "export.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "scene.h"

namespace rx = rxcpp;
namespace rxsc = rxcpp::schedulers;
namespace rxsub = rx::subjects;

using namespace std::chrono;

EXTERN class API D3DSystem
{
public:
    HRESULT InitializeForWindow(bool vsync, HINSTANCE* windowInstance, HWND* windowHandle, bool fullscreen);
    HRESULT InitializeWithWindow(int screenWidth, int screenHeight, bool vsync, bool fullscreen);
    LRESULT MessageHandler(HWND* hwnd, unsigned int umessage, WPARAM wparam, LPARAM lparam);
    HRESULT Initialize();
private:
    HRESULT                 InitializeWindow(int screenWidth, int screenHeight);
    HRESULT                 CreateDevice();
    HRESULT                 CreateInput();
    HRESULT                 GetRenderQualitySettings(ID3D11Device* device);
    vector<MSAA>*           ProduceMsaaCapability(vector<MSAA>* msaaOptions, int i);
    HRESULT                 LoadModels();
    HRESULT                 CreateRenderer();
    HRESULT                 AttachOnRotate();
    HRESULT                 DetachOnRotate();

    //void                    CreateCamera();
    void                    CreateScene();
    void                    CreateViewPort();
    void                    Render();
private:
    POINT                   *_lastPointerPosition;
    shared_ptr<Renderer>    _renderer;
    //shared_ptr<Camera>      _camera;
    shared_ptr<Input>       _inputDevices;
    shared_ptr<Model>       _model;
    Scene                   *_scene;
    D3D_FEATURE_LEVEL       _d3dFeatureLevel;

    XMVECTOR                _position;
    XMVECTOR                _rotation;

    float                   _viewportWidth;
    float                   _viewportHeight;
    float                   _nearZ;
    float                   _farZ;
    bool                    _trackInput;
};
