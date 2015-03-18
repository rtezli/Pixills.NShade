#pragma once

#pragma warning( disable : 4316 )

#include "resource.h"
#include "d3dsystem.h"

HRESULT	Init3DSystem(HWND* pWindow, HINSTANCE* pHInstance);
HWND m_hWnd = 0;
D3DSystem* m_system = 0;