#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "common.h"

using namespace DirectX;


EXTERN class API Input
{
private:
    struct MOVE_ROTATE_INPUT
    {
        XMFLOAT2 Rotate;
        XMFLOAT3 Move;
    };
public:
    HRESULT Initialize();
    HRESULT CreateMouse();
    HRESULT CreateKeyboard();
    HRESULT CreateKinect();
    void(*OnInput)(MOVE_ROTATE_INPUT *move);
private:
    bool Frame();
    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessInput();
    void PublishInput(MOVE_ROTATE_INPUT *move){ (*OnInput)(move); }
private:
    IDirectInput8*          _directInput;
    IDirectInputDevice8*    _keyboard;
    IDirectInputDevice8*    _mouse;
    IDirectInputDevice8*    _kinectMaybe;

    char                    _keyboardState[256];
    DIMOUSESTATE            _mouseState;
    int                     _screenWidth, _screenHeight;
    int                     _mouseX, _mouseY;
    float                   _moveGain;
    float                   _rotateGain;
};

