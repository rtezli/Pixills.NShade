#include "stdafx.h"
#include "input.h"

HRESULT Input::Initialize()
{
    auto result = DirectInput8Create(*Res::Get()->WindowInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (VOID**)&_directInput, NULL);

    if (FAILED(result))
    {
        return result;
    }

    result = CreateKeyboard();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateMouse();
    if (FAILED(result))
    {
        return result;
    }

    result = CreateKinect();
    if (FAILED(result))
    {
        return result;
    }

    return result;
}

HRESULT Input::CreateKeyboard()
{
    auto result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
    if (FAILED(result))
    {
        return result;
    }

    result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result))
    {
        return result;
    }

    result = _keyboard->SetCooperativeLevel(*Res::Get()->WindowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if (FAILED(result))
    {
        return result;
    }

    result = _keyboard->Acquire();
    if (FAILED(result))
    {
        return result;
    }

    return result;
}

HRESULT Input::CreateMouse()
{
    auto result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
    if (FAILED(result))
    {
        return result;
    }

    result = _mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(result))
    {
        return result;
    }

    result = _mouse->SetCooperativeLevel(*Res::Get()->WindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result))
    {
        return result;
    }

    result = _mouse->Acquire();
    if (FAILED(result))
    {
        return result;
    }

    return result;
}

HRESULT Input::CreateKinect()
{
    return 0;
}

BOOL Input::Frame()
{
    BOOL result;


    // Read the current state of the keyboard.
    result = ReadKeyboard();
    if (!result)
    {
        return false;
    }

    // Read the current state of the mouse.
    result = ReadMouse();
    if (!result)
    {
        return false;
    }

    // Process the changes in the mouse and keyboard.
    ProcessInput();

    return true;
}

BOOL Input::ReadKeyboard()
{
    // Read the keyboard device.
    auto result = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            _keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }
    return true;
}

BOOL Input::ReadMouse()
{
    HRESULT result;

    // Read the mouse device.
    result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
    if (FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            _mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

VOID Input::ProcessInput()
{
    // Update the location of the mouse cursor based on the change of the mouse location during the frame.
    _mouseX += _mouseState.lX;
    _mouseY += _mouseState.lY;

    // Ensure the mouse location doesn't exceed the screen width or height.
    if (_mouseX < 0)  { _mouseX = 0; }
    if (_mouseY < 0)  { _mouseY = 0; }

    if (_mouseX > _screenWidth)  { _mouseX = _screenWidth; }
    if (_mouseY > _screenHeight) { _mouseY = _screenHeight; }

    return;
}