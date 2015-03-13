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
	Input(DeviceResources *resources);
public:
	HRESULT Initialize();
	HRESULT CreateMouse();
	HRESULT CreateKeyboard();
	HRESULT CreateKinect();
	void(*OnInput)(MOVE_ROTATE_INPUT *move);
private:
	BOOL	Frame();
	BOOL	ReadKeyboard();
	BOOL	ReadMouse();
	void	ProcessInput();
	void	PublishInput(MOVE_ROTATE_INPUT *move){ (*OnInput)(move); }
private:
	DeviceResources*		_deviceResources;
	IDirectInput8*			_directInput;
	IDirectInputDevice8*	_keyboard;
	IDirectInputDevice8*	_mouse;
	IDirectInputDevice8*	_kinectMaybe;

	CHAR					_keyboardState[256];
	DIMOUSESTATE			_mouseState;
	INT						_screenWidth, _screenHeight;
	INT						_mouseX, _mouseY;
	FLOAT					_moveGain;
	FLOAT					_rotateGain;
};

