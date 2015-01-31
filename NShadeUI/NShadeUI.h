#pragma once

#include "resource.h"
#include "D3DSystem.h"

HRESULT	Init3DSystem(HWND* window);

HWND m_hWnd = 0;
D3DSystem* m_system = 0;