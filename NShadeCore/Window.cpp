#include "stdafx.h"
#include "window.h"


Window::Window(HWND* pWindow, bool vsyncEnabled, bool fullscreen)
{
	m_pWindow = pWindow;
	m_VsyncEnabled = vsyncEnabled;
	m_Fullscreen = fullscreen;
}


Window::~Window()
{

}
