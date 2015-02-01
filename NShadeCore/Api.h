#pragma once

#include "export.h"
#include "d3dsystem.h"

EXTERN HRESULT API InitializeForWindowS(HWND* hwnd);
EXTERN HRESULT API InitializeForWindow(bool vsync, HWND* hwnd, bool fullscreen, float screenDepth, float screenNear);
EXTERN BOOL API AddModel();
EXTERN BOOL API DeleteModel(LPTSTR id);
EXTERN BOOL API AddScene();
EXTERN BOOL API DeleteScene(LPTSTR id);
EXTERN BOOL API AddShader(LPTSTR code);
EXTERN BOOL API DeleteShader(LPTSTR id);
EXTERN BOOL API StartRendering();
EXTERN BOOL API StopRendering();