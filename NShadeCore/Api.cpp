#include "stdafx.h"
#include "api.h"

HRESULT InitializeForWindow(int screenWidth, int screenHeight, BYTE vsync, HWND* hwnd, BYTE fullscreen, float screenDepth, float screenNear)
{
	HRESULT result = 0;
	try
	{
		auto sys = new D3DSystem();
		result = sys->Initialize(screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);
		return result;
	}
	catch (int e)
	{
		return e;
	}

}

BOOL AddModel()
{
	return true;
}

BOOL DeleteModel(CHAR* id)
{
	return true;
}

BOOL AddScene()
{
	return true;
}

BOOL DeleteScene(CHAR* id)
{
	return true;
}

BOOL AddShader(CHAR* code)
{
	return true;
}

BOOL DeleteShader(CHAR* id)
{
	return true;
}

BOOL StartRendering()
{
	return true;
}

BOOL StopRendering()
{
	return true;
}
