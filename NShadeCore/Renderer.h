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

#include "includes.h"
#include "d3dcompiler.h"
#include "window.h"

class Renderer
{
public:
	Renderer(DeviceResources* pResources, std::shared_ptr<Window> pWindow);
	~Renderer();
public:
	void	Render();
	bool	IsTracking() { return m_tracking; }
	bool	IsRendering(){ return m_isRendering; }
public:
	ID3D11Device*				const Device(){ return DeviceResource()->Device; }
	ID3D11DeviceContext*		const DeviceContext(){ return DeviceResource()->DeviceContext; }
	DeviceResources*			const DeviceResource(){ return m_pDeviceResources.get(); }
	ShaderSet*					const Shaders(){ return m_pShaderSet.get(); }
private:
	HRESULT	Initialize();
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

	HRESULT CreateSwapChain();
	void	SetBuffers();

private:
	std::shared_ptr<DeviceResources>		m_pDeviceResources;
	std::shared_ptr<Window>					m_pWindow;
	std::shared_ptr<IDXGIDevice>			m_pDXGIDevice;
	std::shared_ptr<IDXGIAdapter>			m_pDXGIAdapter;
	std::shared_ptr<IDXGIFactory1>			m_pDXGIFactory;
	std::shared_ptr<IDXGISwapChain>			m_pSwapChain;
	std::shared_ptr<ID3D11Texture2D>		m_pRenderBuffer;

	std::shared_ptr<ID3D11InputLayout>		m_pInputLayout;
	std::shared_ptr<ID3D11VertexShader>		m_vertexShader;
	std::shared_ptr<ID3D11PixelShader>		m_pixelShader;

	std::shared_ptr<ID3D11RenderTargetView>	m_pRenderTarget;
	std::shared_ptr<ID3D11DepthStencilView>	m_pDepthStencilView;

	std::shared_ptr<DirectX::XMFLOAT4X4>	m_WorldMatrix;
	std::shared_ptr<DirectX::XMFLOAT4X4>	m_ViewMatrix;
	std::shared_ptr<DirectX::XMFLOAT4X4>	m_ProjectionMatrix;
	std::shared_ptr<ShaderSet>				m_pShaderSet;

	LPCWSTR									m_standardVertexShader = L"VertexShader.cso";
	LPCWSTR									m_standardPixelShader = L"PixelShader.cso";

	bool									m_isRendering;
	bool									m_loadingComplete;
	bool									m_tracking;
};