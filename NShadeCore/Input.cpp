#include "stdafx.h"
#include "input.h"

Input::Input()
{

}

Input::~Input()
{
}

HRESULT Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	auto result = DirectInput8Create(
		hinstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDirectInput, NULL);

	if (FAILED(result))
	{
		return false;
	}

	auto keyboard = m_pKeyboard.get();
	result = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pKeyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	auto mouse = m_pMouse.get();
	result = m_pDirectInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pMouse->SetDataFormat(&c_dfDIMouse);
	result = m_pMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Input::Frame()
{
	bool result;


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

bool Input::ReadKeyboard()
{
	// Read the keyboard device.
	auto result = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_MouseX += m_mouseState.lX;
	m_MouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_MouseX < 0)  { m_MouseX = 0; }
	if (m_MouseY < 0)  { m_MouseY = 0; }

	if (m_MouseX > m_ScreenWidth)  { m_MouseX = m_ScreenWidth; }
	if (m_MouseY > m_ScreenHeight) { m_MouseY = m_ScreenHeight; }

	return;
}