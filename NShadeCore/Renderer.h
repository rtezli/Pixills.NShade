#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "common.h"
#include "export.h"
#include "d3dcompiler.h"
#include "phongvertexshader.h"
#include "scene.h"

enum MSAA : CHAR
{
	SIMPLEST_POSSIBLE = 9,
	MSAA_0X = 0,
	MSAA_1X = 1,
	MSAA_2X = 2,
	MSAA_4X = 4,
	MSAA_8X = 8,
	BEST_POSSIBLE = 8
};

struct RenderingQuality
{
	UINT Quality;
	UINT SampleCount;
	DXGI_FORMAT TextureFormat;
	DXGI_FORMAT BufferFormat;
	BOOL IsMultisamplingSettings;
};

struct MsaaOptions
{
	MSAA Msaa;
	RenderingQuality Quality;
};

struct RENDERER_SETTINGS
{
	DXGI_FORMAT swapChainBufferFormat;
	DXGI_FORMAT depthBufferFormat;
	DXGI_FORMAT depthSencilViewFormat;
	DXGI_FORMAT depthSencilFormat;
	DXGI_FORMAT vertexPositionFormat;
	DXGI_FORMAT vertexColorFormat;
};


EXTERN class API Renderer
{
public:
	Renderer(DeviceResources *pResources, BOOL useSwapChain);
public:
	HRESULT	ResizeSwapChain(UINT32 newWidth, UINT32 newHeight);
	HRESULT SetShaderParameters();
	HRESULT	Initialize(Scene *scene);
	HRESULT	Resize(D3D11_VIEWPORT* viewport);
	HRESULT	Render();

public:
	Scene*						const GetScene(){				return m_pScene.get(); }
	BOOL						const GetInitialized(){			return m_isInitialized; };
private:
	ID3D11Device*				const GetDevice(){ return GetResources()->Device; }
	ID3D11DeviceContext*		const GetDeviceContext(){ return GetResources()->DeviceContext; }
	DeviceResources*			const GetResources(){ return m_pDeviceResources.get(); }
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

	/* shadow map */
	HRESULT CreateShadows();

	HRESULT CreateLights();

	/* rasterizer */
	HRESULT CreateRasterizerDescription();
	HRESULT CreateRasterizer();

	HRESULT CreateViewPort();

private:
	shared_ptr<DeviceResources>			m_pDeviceResources;
	shared_ptr<Scene>					m_pScene;

	RenderingQuality*					m_pRenderingQuality;

	D3D11_TEXTURE2D_DESC				m_pDepthBufferDesc;
	DXGI_SWAP_CHAIN_DESC				m_pSwapChainDescription;

	D3D11_TEXTURE2D_DESC				m_pDepthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC			m_pDepthStencilStateDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC		m_pDepthStencilViewDesc;

	D3D11_TEXTURE2D_DESC				m_pRenderTargetDesc;
	D3D11_RENDER_TARGET_VIEW_DESC		m_pRenderTargetViewDesc;

	D3D11_RASTERIZER_DESC				m_pRasterizerDesc;

	BOOL								m_isInitialized;
	BOOL								m_useSwapChain;
	BOOL								m_renderShadows;
	BOOL								m_rasterizerUseMultiSampling;
};