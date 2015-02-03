#include "stdafx.h"
#include "renderer.h"

Renderer::Renderer(std::shared_ptr<ID3D11Device> pDevice, std::shared_ptr<Window> pWindow)
{
	m_pDevice = pDevice;
	m_pWindow = pWindow;
	Initialize();
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

// 0x 2x, 4x MSAA
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

	swapChainDesc.BufferDesc.Height = m_ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	swapChainDesc.SampleDesc.Count = m_samplesCount; 
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2; 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	//swapChainDesc.Stereo = false;
	//swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	//swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	if (m_pWindow->VSyncEnabled())
	{
		//swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		//swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = (*m_pWindow->WindowHandle().get());

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

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

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

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