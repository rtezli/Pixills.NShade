#pragma once

#include "includes.h"

class Window
{
public:
	Window(HWND* pWindow, bool vsyncEnabled, bool fullscreen);
	~Window();
	bool	Fullscreen()	{ return m_Fullscreen; }
	bool	VSyncEnabled()	{ return m_VsyncEnabled; }
	int		Width()			{ return m_ScreenWidth; }
	int		Height()		{ return m_ScreenHeight; }
	HWND*	WindowHandle()	{ return m_pWindow; }
private :
	HWND*					m_pWindow;
	int						m_ScreenWidth;
	int						m_ScreenHeight;
	bool					m_VsyncEnabled;
	bool					m_Fullscreen;
};

