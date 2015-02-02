#pragma once
#include "memory"
#include "d3dsystem.h"

class System
{
public:
	System();
	~System();
public:
	HRESULT InitializeForWindow(BOOL vsync, HWND* hwnd, BOOL fullscreen, FLOAT screenDepth, FLOAT screenNear);
	HRESULT InitializeWithWindow(INT32 screenWidth, INT32 screenHeight, BOOL vsync, BOOL fullscreen, FLOAT screenDepth, FLOAT screenNear);

	HRESULT AddModelAsFbx(CHAR* fbxFile);
	HRESULT AddModelAsObj(CHAR* objFile);

	HRESULT AddShaderCSO(CHAR* shaderFile);
	HRESULT AddShaderSource(CHAR* shaderFile, byte shaderModel, byte type);
	HRESULT CompileShaderSource(CHAR* shaderFile, byte shaderModel, byte type);

	HRESULT SetPerspective(Perspective* perspective);
	HRESULT SetBackground(XMFLOAT3 perspective);

	HRESULT Input(FLOAT dx, FLOAT dy);
private:
	std::shared_ptr<D3DSystem> m_pSystem;
};

