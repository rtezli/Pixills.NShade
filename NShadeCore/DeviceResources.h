#pragma once
#pragma comment(lib, "D2d1.lib")

#include "common.h"
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
	HINSTANCE*					WindowInstance;
	HWND*						WindowHandle;
	ID3D11Device*				Device;
	ID3D11DeviceContext*		DeviceContext;

	ID3D11Buffer*				VertexBuffer;
	ID3D11Buffer*				IndexBuffer;
	ID3D11Buffer*				ConstBuffer;
	ConstantBufferData*			ConstBufferData;

	ID3D11Texture2D*			BackBuffer;
	ID3D11Texture2D*			DepthStencilBuffer;

	ID3D11DepthStencilState*	DepthStencilState;
	ID3D11RasterizerState*		RasterizerState;

	IDXGISwapChain*				SwapChain;

	ID3D11RenderTargetView*		RenderTargetView;
	ID3D11DepthStencilView*		DepthStencilView;
	ID3D11InputLayout*			InputLayout;

	ShaderSet*					Shaders;

	D3D11_VIEWPORT*				ViewPort;
	RenderingQuality*			RenderQuality;

	int							SwapChainFlags;
	int							BufferCount;
	int							IndexCount;
	int							VertexCount;

	XMVECTORF32					DefaultColor;
	float						Dpi;
	float						NearZ;
	float						FarZ;

	bool						FullScreen;
	bool						VSync;
};

