#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "includes.h"

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
	bool	Frame();
	bool	ReadKeyboard();
	bool	ReadMouse();
	void	ProcessInput();
	void	PublishInput(MOVE_ROTATE_INPUT* move){ (*OnInput)(move); }
private:
	DeviceResources*		m_pDeviceResources;
	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouse;
	IDirectInputDevice8*	m_pKinectMaybe;

	unsigned char			m_keyboardState[256];
	DIMOUSESTATE			m_mouseState;
	int						m_ScreenWidth, m_ScreenHeight;
	int						m_MouseX, m_MouseY;
	float					m_moveGain;
	float					m_rotateGain;
};

