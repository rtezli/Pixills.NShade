#pragma once
#pragma comment(lib, "D2d1.lib")

#include "includes.h"
#include "directxcolors.h"
#include "dxgi1_2.h"
#include "d2d1_1.h"

using namespace std;
using namespace DirectX;

class DeviceResources
{

public:
	DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DeviceResources();
public:
	HWND*						WindowHandle;
	ID3D11Device*				Device;
	ID3D11DeviceContext*		DeviceContext;

	ID3D11Buffer*				VertexBuffer;
	ID3D11Buffer*				IndexBuffer;
	ID3D11Buffer*				ConstBuffer;
	ConstantBufferData*			ConstBufferData;

	IDXGISwapChain*				SwapChain;
	IDXGISurface*				DxgiBackBuffer;
	ID2D1Bitmap*				D2dTargetBitmap;

	ID3D11RenderTargetView*		RenderTargetView;
	ID3D11DepthStencilView*		DepthStencilView;
	ID3D11InputLayout*			InputLayout;
	XMVECTORF32					DefaultColor;
	ShaderSet*					Shaders;

	D3D11_VIEWPORT				ViewPort;
	int							SwapChainFlags;
	int							Quality;
	int							SamplesCount;
	int							BufferCount;
	int							IndexCount;
	int							ScreenWidth;
	int							ScreenHeight;

	float						Dpi;

	float						NearZ;
	float						FarZ;
	bool						FullScreen;
	bool						VSync;
};

