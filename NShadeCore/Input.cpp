#include "stdafx.h"
#include "input.h"

Input::Input(DeviceResources* pDeviceResources)
{
	m_pDeviceResources = pDeviceResources;
}

Input::~Input()
{
}

HRESULT Input::Initialize()
{
	auto result = DirectInput8Create(
		*m_pDeviceResources->WindowInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDirectInput, nullptr);

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
	auto result = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr);
	if (FAILED(result))
	{
		return result;
	}

	result = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return result;
	}

	result = m_pKeyboard->SetCooperativeLevel(*m_pDeviceResources->WindowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return result;
	}

	result = m_pKeyboard->Acquire();
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Input::CreateMouse()
{
	auto result = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);
	if (FAILED(result))
	{
		return result;
	}

	result = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return result;
	}

	result = m_pMouse->SetCooperativeLevel(*m_pDeviceResources->WindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return result;
	}

	result = m_pMouse->Acquire();
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

BOOL Input::ReadMouse()
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

VOID Input::ProcessInput()
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