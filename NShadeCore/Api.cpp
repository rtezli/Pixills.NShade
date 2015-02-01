#include "stdafx.h"
#include "api.h"

HRESULT InitializeForWindow(BOOL vsync, HWND* hwnd, BOOL fullscreen, FLOAT screenDepth, FLOAT screenNear)
{
	HRESULT result = 0;
	try
	{
		auto sys = new D3DSystem();
		result = sys->Initialize(vsync, hwnd, fullscreen, screenDepth, screenNear);
	}
	catch (int e)
	{
		result = e;
	}
	return result;
}

BOOL AddModel()
{
	return true;
}

BOOL DeleteModel(LPTSTR id)
{
	return true;
}

BOOL AddScene()
{
	return true;
}

BOOL DeleteScene(LPTSTR id)
{
	return true;
}

BOOL AddShader(LPTSTR code)
{
	return true;
}

BOOL DeleteShader(LPTSTR id)
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
