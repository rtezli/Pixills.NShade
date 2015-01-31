#pragma once

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "dinput.h"

class Input
{
public:
	Input();
	~Input();
public:
	HRESULT Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
private:
	BOOL Input::Frame();
	BOOL Input::ReadKeyboard();
	BOOL Input::ReadMouse();
	void Input::ProcessInput();
private:
	IDirectInput8* m_pDirectInput = 0;
	IDirectInputDevice8* m_pKeyboard = 0;
	IDirectInputDevice8* m_pMouse = 0;
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_ScreenWidth, m_ScreenHeight;
	int m_MouseX, m_MouseY;
};

