#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "memory"
#include "export.h"
#include "dinput.h"
#include "directxmath.h"
//#include "rx.hpp"

//namespace rx = rxcpp;
//namespace rxsub = rxcpp::subjects;
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
	Input();
	~Input();
public:
	HRESULT Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void(*OnInput)(MOVE_ROTATE_INPUT* move);
private:
	bool Input::Frame();
	bool Input::ReadKeyboard();
	bool Input::ReadMouse();
	void Input::ProcessInput();
	void Input::PublishInput(MOVE_ROTATE_INPUT* move){ (*OnInput)(move); }
private:
	std::shared_ptr<IDirectInput8> m_pDirectInput = 0;
	std::shared_ptr<IDirectInputDevice8> m_pKeyboard = 0;
	std::shared_ptr<IDirectInputDevice8> m_pMouse = 0;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	int m_ScreenWidth, m_ScreenHeight;
	int m_MouseX, m_MouseY;
};

