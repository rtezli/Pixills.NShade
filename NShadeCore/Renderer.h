#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#ifndef PS_PROFILE
#define PS_PROFILE = "ps_5_0"
#endif

#ifndef VS_PROFILE
#define VS_PROFILE = "vs_5_0"
#endif

#ifndef GS_PROFILE
#define GS_PROFILE = "ps_5_0"
#endif

#ifndef HS_PROFILE
#define HS_PROFILE = "hs_5_0"
#endif

#ifndef DS_PROFILE
#define DS_PROFILE = "ds_5_0"
#endif

#include "includes.h"
#include "d3dcompiler.h"
#include "window.h"

class Renderer
{
public:
	Renderer(std::shared_ptr<ID3D11Device> pDevice, std::shared_ptr<Window> pWindow);
	~Renderer();
public:
	bool	IsTracking() { return m_tracking; }
	bool	IsRendering(){ return m_isRendering; }
private:
	void	Initialize();

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
	void	Render();

private:
	std::shared_ptr<Window>					m_pWindow;
	std::shared_ptr<ID3D11Device>			m_pDevice;
	std::shared_ptr<IDXGIDevice>			m_pDXGIDevice;
	std::shared_ptr<IDXGIAdapter>			m_pDXGIAdapter;
	std::shared_ptr<IDXGIFactory1>			m_pDXGIFactory;
	std::shared_ptr<IDXGISwapChain>			m_pSwapChain;
	std::shared_ptr<ID3D11Texture2D>		m_pRenderBuffer;

	std::shared_ptr<ID3D11InputLayout>		m_inputLayout;
	std::shared_ptr<ID3D11Buffer>			m_vertexBuffer;
	std::shared_ptr<ID3D11Buffer>			m_indexBuffer;
	std::shared_ptr<ID3D11VertexShader>		m_vertexShader;
	std::shared_ptr<ID3D11PixelShader>		m_pixelShader;
	std::shared_ptr<ID3D11Buffer>			m_constantBuffer;

	std::shared_ptr<ID3D11RenderTargetView>	m_pRenderTarget;
	std::shared_ptr<ID3D11DepthStencilView>	m_pDepthStencilView;

	std::shared_ptr<DirectX::XMFLOAT4X4>	m_WorldMatrix;
	std::shared_ptr<DirectX::XMFLOAT4X4>	m_ViewMatrix;
	std::shared_ptr<DirectX::XMFLOAT4X4>	m_ProjectionMatrix;

	int										m_samplesCount = 4;

	struct SHADER_SET*						m_pShaderSet;

	char*									m_standardVertexShader = "PixelShader.cso";
	char*									m_standardPixelShader = "VertexShader.cso";

	int										m_ScreenWidth;
	int										m_ScreenHeight;

	bool									m_isRendering;
	bool									m_loadingComplete;
	bool									m_tracking;
};

struct SHADER_SET
{
	std::shared_ptr<ID3D11PixelShader>		PixelShader;
	std::shared_ptr<ID3D11VertexShader>		VertexShader;
	std::shared_ptr<ID3D11HullShader>		HullShader;
	std::shared_ptr<ID3D11DomainShader>		DomainShader;
	std::shared_ptr<ID3D11GeometryShader>	GeometryShader;
	std::shared_ptr<ID3D11ComputeShader>	ComputeShader;
};