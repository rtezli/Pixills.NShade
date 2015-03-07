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
	DeviceResources(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
public:
	HINSTANCE*					WindowInstance;
	HWND*						WindowHandle;
	ID3D11Device*				Device;
	ID3D11DeviceContext*		DeviceContext;
	ID3D11RasterizerState*		RasterizerState;
	IDXGISwapChain*				SwapChain;

	/* depth stencil */
	ID3D11Texture2D*			DepthStencilBuffer;
	ID3D11DepthStencilState*	DepthStencilState;
	ID3D11DepthStencilView*		DepthStencilView;

	/* render target (screen) */
	ID3D11Texture2D*			BackBuffer;
	ID3D11RenderTargetView*		RenderTargetView;

	/* shadows */
	ID3D11Texture2D*			ShadowTexture;
	ID3D11RenderTargetView*		ShadowRenderTarget;
	ID3D11ShaderResourceView*	ShadowResourceView;

	/* mirrors */
	ID3D11Texture2D*			MirrorTexture;
	ID3D11RenderTargetView*		MirrorRenderTarget;
	ID3D11ShaderResourceView*	MirrorResourceView;

	D3D11_VIEWPORT*				ViewPort;

	INT							SwapChainFlags;
	INT							BufferCount;
	FLOAT						Dpi;
	FLOAT						NearZ;
	FLOAT						FarZ;

	BOOL						FullScreen;
	BOOL						VSync;
};

