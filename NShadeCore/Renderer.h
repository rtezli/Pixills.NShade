#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#ifndef PS_PROFILE
#define PS_PROFILE {"ps_5_0" }
#endif

#ifndef VS_PROFILE
#define VS_PROFILE {"vs_5_0" }
#endif

#ifndef GS_PROFILE
#define GS_PROFILE { "ps_5_0" }
#endif

#ifndef HS_PROFILE
#define HS_PROFILE { "hs_5_0" }
#endif

#ifndef DS_PROFILE
#define DS_PROFILE { "ds_5_0" }
#endif

#include "common.h"
#include "d3dcompiler.h"
#include "phongvertexshader.h"

EXTERN class API Renderer
{
public:
	Renderer(DeviceResources *resources, BOOL useSwapChain);
public:
	HRESULT	ResizeSwapChain(UINT32 newWidth, UINT32 newHeight);
	HRESULT SetShaderParameters();
	HRESULT	Initialize();
	HRESULT	Render();
	HRESULT	Resize(D3D11_VIEWPORT* viewport);
public:
	VOID						ClearScene();
	BOOL						const Initialized(){ return _isInitialized; };
private:
	/* render target */
	HRESULT CreateRenderTargetDesciption();
	HRESULT CreateRenderTargetViewDesciption();
	HRESULT CreateRenderTarget();

	/* swap chain */
	HRESULT	CreateSwapChainDesciption();
	HRESULT CreateSwapChain();

	/* constant buffer */
	HRESULT CreateConstantBufferDescription();
	HRESULT CreateConstantBuffer();

	/* depth buffer */
	HRESULT CreateDepthBufferDescription();
	HRESULT CreateDepthBuffer();

	/* depth stencil */
	HRESULT CreateDepthStencilDescription();
	HRESULT CreateDepthStencilViewDescription();
	HRESULT CreateDepthStencilStateDescription();
	HRESULT CreateDepthStencil();

	/* rasterizer */
	HRESULT CreateRasterizerDescription();
	HRESULT CreateRasterizer();

	HRESULT CreateViewPort();

	/* sahders */
	HRESULT	SetVertexShader(LPCWSTR compiledShaderFile);
	HRESULT	CompileVertexShader(LPCWSTR shaderSource);

	HRESULT SetHullShader(LPCWSTR compiledShaderFile);
	HRESULT CompileHullShader(LPCWSTR shaderSource);

	HRESULT SetDomainShader(LPCWSTR compiledShaderFile);
	HRESULT CompileDomainShader(LPCWSTR shaderSource);

	HRESULT SetGeometryShader(LPCWSTR compiledShaderFile);
	HRESULT CompileGeometryShader(LPCWSTR shaderSource);

	HRESULT SetPixelShader(LPCWSTR compiledShaderFile);
	HRESULT CompilePixelShader(LPCWSTR shaderSource);

	HRESULT CompileShader(LPCWSTR compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile);
private:
	DeviceResources						*_resources;
	LPCWSTR								_standardVertexShader = L"../Debug/PhongVertexShader.cso";
	LPCWSTR								_standardPixelShader = L"../Debug/PhongPixelShader.cso";

	D3D11_TEXTURE2D_DESC				_depthBufferDesc;
	DXGI_SWAP_CHAIN_DESC				_swapChainDescription;

	D3D11_TEXTURE2D_DESC				_depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC			_depthStencilStateDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC		_depthStencilViewDesc;

	D3D11_TEXTURE2D_DESC				_renderTargetDesc;
	D3D11_RENDER_TARGET_VIEW_DESC		_renderTargetViewDesc;

	D3D11_RASTERIZER_DESC				_rasterizerDesc;

	BOOL								_isInitialized;
	BOOL								_useSwapChain;
	BOOL								_rasterizerUseMultiSampling;
};