#include "stdafx.h"
#include "renderer.h"

Renderer::Renderer(std::shared_ptr<ID3D11Device> pDevice, std::shared_ptr<Window> pWindow)
{
	m_pDevice = pDevice;
	m_pWindow = pWindow;
	m_pShaderSet = new SHADER_SET();
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
	SetVertexShader(m_standardVertexShader);
	SetPixelShader(m_standardPixelShader);
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

HRESULT Renderer::SetVertexShader(LPCWSTR compiledShaderFile)
{
	ID3D11VertexShader* vertexShader = 0;
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = m_pDevice->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &vertexShader);
	m_pShaderSet->VertexShader = std::shared_ptr<ID3D11VertexShader>(vertexShader);
	return result;
}

HRESULT Renderer::CompileVertexShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11VertexShader* vertexShader = 0;
	auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
	if (SUCCEEDED(result))
	{
		result = m_pDevice->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
	}
	return result;
}

HRESULT Renderer::SetHullShader(LPCWSTR compiledShaderFile)
{
	ID3D11HullShader* hullShader = 0;
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = m_pDevice->CreateHullShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &hullShader);
	m_pShaderSet->HullShader = std::shared_ptr<ID3D11HullShader>(hullShader);
	return result;
}

HRESULT Renderer::CompileHullShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11HullShader* hullShader = 0;
	auto result = CompileShader(compiledShaderFile, shaderBlob, HS_PROFILE);
	if (SUCCEEDED(result))
	{
		result = m_pDevice->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &hullShader);
	}
	return result;
}

HRESULT Renderer::SetGeometryShader(LPCWSTR compiledShaderFile)
{
	ID3D11GeometryShader* geometryShader = 0;
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = m_pDevice->CreateGeometryShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &geometryShader);
	m_pShaderSet->GeometryShader = std::shared_ptr<ID3D11GeometryShader>(geometryShader);
	return result;
}

HRESULT Renderer::CompileGeometryShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11GeometryShader* geometryShader = 0;
	auto result = CompileShader(compiledShaderFile, shaderBlob, GS_PROFILE);
	if (SUCCEEDED(result))
	{
		result = m_pDevice->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &geometryShader);
	}
	return result;
}

HRESULT Renderer::SetPixelShader(LPCWSTR compiledShaderFile)
{
	ID3D11PixelShader* pixelShader = 0;
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = m_pDevice->CreatePixelShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &pixelShader);
	m_pShaderSet->PixelShader = std::shared_ptr<ID3D11PixelShader>(pixelShader);
	return result;
}

HRESULT Renderer::CompilePixelShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11PixelShader* pixelShader = 0;
	auto result = CompileShader(compiledShaderFile, shaderBlob, PS_PROFILE);
	if (SUCCEEDED(result))
	{
		result = m_pDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
	}
	return result;
}

HRESULT Renderer::CompileShader(LPCWSTR compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile)
{
	ID3DBlob* shaderBlob = 0;
 
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", 
		"1",
		NULL,
		NULL
	};

	auto result = D3DCompileFromFile(
		compiledShaderFile,
		NULL, // Defines
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", // Entrypoint
		shaderProfile,
		flags,
		0,
		&shaderBlob,
		&shaderBlob);
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