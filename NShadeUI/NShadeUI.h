#pragma once

#pragma warning( disable : 4316 )

#include "resource.h"
//#include "D3DSystem.h"
#include "api.h"
HRESULT	Init3DSystem(HWND* window);

HWND m_hWnd = 0;
D3DSystem* m_system = 0;