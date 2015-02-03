#include "stdafx.h"
#include "renderer.h"

Renderer::Renderer(std::shared_ptr<ID3D11Device> pDevice, std::shared_ptr<Window> pWindow)
{
	m_pDevice = pDevice;
	m_pWindow = pWindow;
}

Renderer::~Renderer()
{
	m_pRenderTarget->Release();
	m_pDepthStencilView->Release();
}

void Renderer::Initialize()
{
	CreateSwapChain();
	InitializeShaders();
}

HRESULT Renderer::CreateSwapChain()
{
	IDXGIDevice* dxgiDevice = 0;
	IDXGIAdapter* dxgiAdapter = 0;
	IDXGIFactory1* dxgiFactory = 0;

	auto result = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter);
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), (void **)&dxgiFactory);

	m_pDXGIDevice = std::shared_ptr<IDXGIDevice>(dxgiDevice);
	m_pDXGIAdapter = std::shared_ptr<IDXGIAdapter>(dxgiAdapter);
	m_pDXGIFactory = std::shared_ptr<IDXGIFactory1>(dxgiFactory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = m_ScreenWidth;
	swapChainDesc.BufferDesc.Height = m_ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_pWindow->VSyncEnabled())
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		//swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		//swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = (*m_pWindow->WindowHandle().get());

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (m_pWindow->Fullscreen())
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	auto device = m_pDevice.get();
	auto swapChain = m_pSwapChain.get();
	result = m_pDXGIFactory->CreateSwapChain(device, &swapChainDesc, &swapChain);
	return 0;
}

HRESULT Renderer::InitializeShaders()
{
	m_pShaderSet = new SHADER_SET();
	
	ID3D11PixelShader* pixelShader = 0;
	auto psByteCode = File::ReadFileBytes(m_standardPixelShader);
	auto result = m_pDevice->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, NULL, &pixelShader);
	m_pShaderSet->PixelShader = std::shared_ptr<ID3D11PixelShader>(pixelShader);

	ID3D11VertexShader* vertexShader = 0;
	auto vsByteCode = File::ReadFileBytes(m_standardVertexShader);
	result = m_pDevice->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &vertexShader);
	m_pShaderSet->VertexShader = std::shared_ptr<ID3D11VertexShader>(vertexShader);

	return result;
}

void Renderer::Render()
{
	ID3D11Texture2D* backBuffer = 0;
	ID3D11RenderTargetView* targetView = 0;
	auto swapChain = m_pSwapChain.get();
	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &targetView);
	swapChain->Present(1, 0);
}