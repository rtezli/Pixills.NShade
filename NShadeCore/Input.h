#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "common.h"

using namespace DirectX;


class Input
{
private:
	struct MOVE_ROTATE_INPUT
	{
		XMFLOAT2 Rotate;
		XMFLOAT3 Move;
	};
public:
	Input(DeviceResources* pDeviceResources);
	~Input();
public:
	HRESULT Initialize();
	HRESULT CreateMouse();
	HRESULT CreateKeyboard();
	HRESULT CreateKinect();
	void(*OnInput)(MOVE_ROTATE_INPUT* move);
private:
	BOOL	Frame();
	BOOL	ReadKeyboard();
	BOOL	ReadMouse();
	VOID	ProcessInput();
	VOID	PublishInput(MOVE_ROTATE_INPUT* move){ (*OnInput)(move); }
private:
	DeviceResources*		m_pDeviceResources;
	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouse;
	IDirectInputDevice8*	m_pKinectMaybe;

	UINT					m_keyboardState[256];
	DIMOUSESTATE			m_mouseState;
	INT						m_ScreenWidth, m_ScreenHeight;
	INT						m_MouseX, m_MouseY;
	FLOAT					m_moveGain;
	FLOAT					m_rotateGain;
};

