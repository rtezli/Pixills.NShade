#pragma once

#include "includes.h"

class Screen
{
public:
	Screen(std::shared_ptr<HWND> window);
	~Screen();
	BOOL Fullscreen(){ return m_Fullscreen; }
	BOOL VSync(){ return m_vsync_enabled; }
	INT32 Width(){ return m_ScreenWidth; }
	INT32 Height(){ return m_ScreenHeight; }
	std::shared_ptr<HWND> WindowHandle(){ return m_pWindow; }
private :
	std::shared_ptr<HWND>	m_pWindow;
	INT32					m_ScreenWidth;
	INT32					m_ScreenHeight;
	BOOL					m_vsync_enabled;
	BOOL					m_Fullscreen;
};

