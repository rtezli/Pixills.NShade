#include "stdafx.h"
#include "d3dsystem.h"

NShade::D3DSystem::D3DSystem()
{
}

NShade::D3DSystem::~D3DSystem()
{
}

HRESULT NShade::D3DSystem::InitializeWithWindow(
	int screenWidth,
	int screenHeight,
	bool vsync,
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	HWND windowHandle = 0;
	Initialize(screenWidth, screenHeight, vsync, windowHandle, fullscreen, screenDepth, screenNear);
	return 0;
}

HRESULT NShade::D3DSystem::Initialize(
	int screenWidth,
	int screenHeight,
	bool vsync,
	HWND hwnd,
	bool fullscreen,
	float screenDepth,
	float screenNear)
{
	CreateDevice();
	CreateCamera();
	LoadModels();
	CreateRenderer();

	return 0;
}

HRESULT NShade::D3DSystem::CreateDevice()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	ID3D11Device* pDevice;
	ID3D11DeviceContext*  pDeviceContext;

	//HRESULT createResult = D3D11CreateDevice(
	//	nullptr,					// "nullptr" angeben, um den Standardadapter zu verwenden.
	//	D3D_DRIVER_TYPE_HARDWARE,	// Mit dem Hardwaregrafiktreiber ein Gerät erstellen.
	//	0,							// Sollte 0 sein, wenn der Treiber nicht D3D_DRIVER_TYPE_SOFTWARE ist.
	//	creationFlags,				// Debug- und Direct2D-Kompatibilitätsflags festlegen.
	//	featureLevels,				// Liste der von dieser App unterstützten Funktionsebenen.
	//	ARRAYSIZE(featureLevels),	// Größe der oben angeführten Liste.
	//	D3D11_SDK_VERSION,			// Dies für Windows Store-Apps immer auf D3D11_SDK_VERSION festlegen.
	//	&pDevice,					// Gibt das erstellte Direct3D-Gerät zurück.
	//	&m_d3dFeatureLevel,			// Gibt die Funktionsebene des erstellten Geräts zurück.
	//	&pDeviceContext				// Gibt den unmittelbaren Kontext des Geräts zurück.
	//	);
	//if (FAILED(createResult))
	//{
	//}

	return 0;
}

HRESULT SetCamera(XMVECTOR position, XMVECTOR direction)
{
	return 0;
}

HRESULT SetCamera(XMVECTOR position, XMVECTOR direction, FLOAT focalLength)
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateSwapChain()
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateCamera()
{
	return 0;
}

HRESULT NShade::D3DSystem::LoadModels()
{
	return 0;
}

HRESULT NShade::D3DSystem::CreateRenderer()
{
	return 0;
}

HRESULT NShade::D3DSystem::ApplyShaders()
{
	return 0;
}

HRESULT NShade::D3DSystem::Render()
{
	// TODO : limit FPS to a value
	while (m_renderer->IsRendering())
	{
		m_renderer->Render();
	}
	return 0;
}
