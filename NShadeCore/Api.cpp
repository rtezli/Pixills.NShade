#include "stdafx.h"
#include "api.h"

BOOL InitializeForWindow(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	auto sys = new D3DSystem();
	auto result = sys->Initialize(screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);
	return result;
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
