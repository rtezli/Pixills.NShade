#include "stdafx.h"
#include "window.h"


Window::Window(std::shared_ptr<HWND> pWindow, bool vsyncEnabled, bool fullscreen)
{
	m_pWindow = pWindow;
	m_VsyncEnabled = vsyncEnabled;
	m_Fullscreen = fullscreen;

	auto hwnd = pWindow.get();
	
	RECT windowRect;
	auto result = GetWindowRect(*hwnd, &windowRect);
	if (SUCCEEDED(result))
	{
		m_ScreenWidth = windowRect.right - windowRect.left;
		m_ScreenHeight = windowRect.bottom - windowRect.top;
	}
}


Window::~Window()
{

}
