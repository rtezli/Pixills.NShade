#pragma once

#include "d3dsystem.h"

EXTERN HRESULT API InitializeForWindow(int screenWidth, int screenHeight, BYTE vsync, HWND hwnd, BYTE fullscreen, float screenDepth, float screenNear);
EXTERN BOOL API AddModel();
EXTERN BOOL API DeleteModel(CHAR* id);
EXTERN BOOL API AddScene();
EXTERN BOOL API DeleteScene(CHAR* id);
EXTERN BOOL API AddShader(CHAR* code);
EXTERN BOOL API DeleteShader(CHAR* id);
EXTERN BOOL API StartRendering();
EXTERN BOOL API StopRendering();