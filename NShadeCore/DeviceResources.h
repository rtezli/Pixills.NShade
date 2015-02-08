#pragma once

#include "includes.h"

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
	ID3D11Buffer*				ConstantBuffer;
	MVPConstantBuffer*			ConstantBufferData;

	ID3D11RenderTargetView*		RenderTargetView;
	ID3D11DepthStencilView*		DepthStencilView;

	ShaderSet*					Shaders;

	D3D11_VIEWPORT				ViewPort;
	int							SamplesCount;
	int							IndexCount;
	int							ScreenWidth;
	int							ScreenHeight;
	float						NearZ;
	int							FarZ;
	bool						FullScreen;
	bool						VSync;
};

