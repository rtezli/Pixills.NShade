#include "stdafx.h"
#include "renderer.h"


Renderer::Renderer(DeviceResources* pResources, bool useSwapChain)
{
	m_isInitialized = false;
	m_pDeviceResources = pResources;
	m_pDeviceResources->Shaders = new ShaderSet();
	m_useSwapChain = useSwapChain;
	m_rasterizerUseMultiSampling = true;

	DXGI_FORMAT swapChainBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT depthSencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT depthSencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DXGI_FORMAT vertexPositionFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	DXGI_FORMAT vertexColorFormat = DXGI_FORMAT_R32G32B32_FLOAT;
}

Renderer::~Renderer()
{

}

HRESULT Renderer::Initialize()
{
	auto result = CreateSwapChain();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateRenderTarget();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateDepthBuffer();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateDepthStencil();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateRasterizer();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateViewPort();
	if (FAILED(result))
	{
		return result;
	}

	result = SetVertexShader(m_standardVertexShader);
	if (FAILED(result))
	{
		return result;
	}
	m_isInitialized = true;
	return SetPixelShader(m_standardPixelShader);
}


HRESULT Renderer::CreateRenderTargetDesciption()
{
	ZeroMemory(&m_pRenderTargetDesc, sizeof(m_pRenderTargetDesc));

	m_pRenderTargetDesc.Width = Resources()->ViewPort->Width;
	m_pRenderTargetDesc.Height = Resources()->ViewPort->Height;
	m_pRenderTargetDesc.MipLevels = 1;
	m_pRenderTargetDesc.ArraySize = 1;
	m_pRenderTargetDesc.Format = Resources()->RenderQuality->TextureFormat;
	m_pRenderTargetDesc.SampleDesc.Quality = Resources()->RenderQuality->Quality;
	m_pRenderTargetDesc.SampleDesc.Count = Resources()->RenderQuality->SampleCount;
	m_pRenderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
	m_pRenderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	m_pRenderTargetDesc.CPUAccessFlags = 0;
	m_pRenderTargetDesc.MiscFlags = 0;

	ID3D11Texture2D* renderTarget = 0;

	return GetDevice()->CreateTexture2D(&m_pRenderTargetDesc, NULL, &renderTarget);
}

HRESULT Renderer::CreateRenderTargetViewDesciption()
{
	ZeroMemory(&m_pRenderTargetViewDesc, sizeof(m_pRenderTargetViewDesc));
	m_pRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	m_pRenderTargetViewDesc.Format = m_pRenderTargetDesc.Format;
	return 0;
}

HRESULT Renderer::CreateRenderTarget()
{
	auto result = CreateRenderTargetDesciption();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateRenderTargetViewDesciption();
	if (FAILED(result))
	{
		return result;
	}

	return GetDevice()->CreateRenderTargetView(Resources()->BackBuffer, &m_pRenderTargetViewDesc, &Resources()->RenderTargetView);
}


HRESULT Renderer::CreateSwapChainDesciption()
{
	ZeroMemory(&m_pSwapChainDescription, sizeof(m_pSwapChainDescription));

	m_pSwapChainDescription.BufferCount = Resources()->BufferCount;
	m_pSwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	m_pSwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_pSwapChainDescription.Flags = Resources()->SwapChainFlags;

	m_pSwapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	m_pSwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	m_pSwapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_pSwapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	m_pSwapChainDescription.SampleDesc.Quality = Resources()->RenderQuality->Quality;
	m_pSwapChainDescription.SampleDesc.Count = Resources()->RenderQuality->SampleCount;
	m_pSwapChainDescription.BufferDesc.Format = Resources()->RenderQuality->TextureFormat;
	m_pSwapChainDescription.BufferDesc.Width = Resources()->ViewPort->Width;
	m_pSwapChainDescription.BufferDesc.Height = Resources()->ViewPort->Height;

	auto handle = *Resources()->WindowHandle;
	m_pSwapChainDescription.OutputWindow = handle;

	if (Resources()->FullScreen)
	{
		m_pSwapChainDescription.Windowed = false;
	}
	else
	{
		m_pSwapChainDescription.Windowed = true;
	}
	return 0;
}

HRESULT Renderer::CreateSwapChain()
{
	auto result = CreateSwapChainDesciption();
	if (FAILED(result))
	{
		return result;
	}

	IDXGIDevice* dxgiDevice = 0;
	IDXGIAdapter* dxgiAdapter = 0;
	IDXGIFactory* dxgiFactory = 0;

	result = GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiFactory->CreateSwapChain(GetDevice(), &m_pSwapChainDescription, &Resources()->SwapChain);
	if (FAILED(result))
	{
		return result;
	}

	result = Resources()->SwapChain->GetBuffer(0, IID_PPV_ARGS(&Resources()->BackBuffer));
	if (FAILED(result))
	{
		return result;
	}

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	return 0;
}


HRESULT Renderer::CreateDepthBufferDescription()
{
	ZeroMemory(&m_pDepthBufferDesc, sizeof(m_pDepthBufferDesc));

	m_pDepthBufferDesc.Width = Resources()->ViewPort->Width;
	m_pDepthBufferDesc.Height = Resources()->ViewPort->Height;
	m_pDepthBufferDesc.MipLevels = 1;
	m_pDepthBufferDesc.ArraySize = 1;
	m_pDepthBufferDesc.Format = Resources()->RenderQuality->BufferFormat;
	m_pDepthBufferDesc.SampleDesc.Quality = Resources()->RenderQuality->Quality;
	m_pDepthBufferDesc.SampleDesc.Count = Resources()->RenderQuality->SampleCount;
	m_pDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_pDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_pDepthBufferDesc.CPUAccessFlags = 0;
	m_pDepthBufferDesc.MiscFlags = 0;

	return 0;
}

HRESULT Renderer::CreateDepthBuffer()
{

	auto result = CreateDepthBufferDescription();
	if (FAILED(result))
	{
		return result;
	}
	return GetDevice()->CreateTexture2D(&m_pDepthBufferDesc, NULL, &Resources()->DepthStencilBuffer);
}


HRESULT Renderer::CreateDepthStencilDescription()
{
	ZeroMemory(&m_pDepthStencilDesc, sizeof(m_pDepthStencilDesc));

	m_pDepthStencilDesc.ArraySize = 1;
	m_pDepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_pDepthStencilDesc.MipLevels = Resources()->RenderQuality->Quality > 0 ? 1 : 0;
	m_pDepthStencilDesc.SampleDesc.Quality = Resources()->RenderQuality->Quality;
	m_pDepthStencilDesc.SampleDesc.Count = Resources()->RenderQuality->SampleCount;
	m_pDepthStencilDesc.Format = Resources()->RenderQuality->BufferFormat;
	m_pDepthStencilDesc.Width = Resources()->ViewPort->Width;
	m_pDepthStencilDesc.Height = Resources()->ViewPort->Height;

	return 0;
}

HRESULT Renderer::CreateDepthStencilStateDescription()
{
	ZeroMemory(&m_pDepthStencilStateDesc, sizeof(m_pDepthStencilStateDesc));

	m_pDepthStencilStateDesc.DepthEnable = true;
	m_pDepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_pDepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	m_pDepthStencilStateDesc.StencilEnable = true;
	m_pDepthStencilStateDesc.StencilReadMask = 0xFF;
	m_pDepthStencilStateDesc.StencilWriteMask = 0xFF;

	m_pDepthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_pDepthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_pDepthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_pDepthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_pDepthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_pDepthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	m_pDepthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_pDepthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	auto result = GetDevice()->CreateDepthStencilState(&m_pDepthStencilStateDesc, &Resources()->DepthStencilState);

	GetDeviceContext()->OMSetDepthStencilState(Resources()->DepthStencilState, 1);
	return result;
}

HRESULT Renderer::CreateDepthStencilViewDescription()
{
	ZeroMemory(&m_pDepthStencilViewDesc, sizeof(m_pDepthStencilViewDesc));

	m_pDepthStencilViewDesc.Format = Resources()->RenderQuality->BufferFormat;
	m_pDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	m_pDepthStencilViewDesc.Texture2D.MipSlice = 0;

	return 0;
}

HRESULT Renderer::CreateDepthStencil()
{
	auto result = CreateDepthStencilDescription();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateDepthStencilViewDescription();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateDepthStencilStateDescription();
	if (FAILED(result))
	{
		return result;
	}

	ID3D11Texture2D* depthStencil = 0;

	result = GetDevice()->CreateTexture2D(&m_pDepthStencilDesc, NULL, &depthStencil);
	result = GetDevice()->CreateDepthStencilView(depthStencil, &m_pDepthStencilViewDesc, &Resources()->DepthStencilView);

	if (FAILED(result))
	{
		return result;
	}

	// D2D1_BITMAP_PROPERTIES1 bitmapProperties =
	// D2D1::BitmapProperties1(
	// D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
	// D2D1::PixelFormat(Resources()->RenderQuality->TextureFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
	// Resources()->Dpi,
	// Resources()->Dpi);

	// IDXGISurface2* dxgiBackBuffer;
	// Resources()->SwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	// ID2D1Bitmap1* targetBitmap;
	// GetDeviceContext()->CreateBitmapFromDxgiSurface(dxgiBackBuffer,&bitmapProperties,&targetBitmap);
	// GetDeviceContext()->SetTarget(targetBitmap);
	// GetDeviceContext()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	return 0;
}


HRESULT Renderer::CreateRasterizerDescription()
{
	ZeroMemory(&m_pRasterizerDesc, sizeof(m_pRasterizerDesc));

	m_pRasterizerDesc.AntialiasedLineEnable = m_rasterizerUseMultiSampling;
	m_pRasterizerDesc.CullMode = D3D11_CULL_NONE;// D3D11_CULL_BACK; //
	m_pRasterizerDesc.DepthBias = 0;
	m_pRasterizerDesc.DepthBiasClamp = 0.0f;
	m_pRasterizerDesc.DepthClipEnable = true;
	m_pRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_pRasterizerDesc.FrontCounterClockwise = false;
	m_pRasterizerDesc.MultisampleEnable = m_rasterizerUseMultiSampling;
	m_pRasterizerDesc.ScissorEnable = false;
	m_pRasterizerDesc.SlopeScaledDepthBias = 0.0f;

	return 0;
}

HRESULT Renderer::CreateRasterizer()
{
	auto result = CreateRasterizerDescription();
	if (FAILED(result))
	{
		return result;
	}

	result = GetDevice()->CreateRasterizerState(&m_pRasterizerDesc, &Resources()->RasterizerState);
	if (FAILED(result))
	{
		return result;
	}

	// Set rasterizer
	ID3D11RasterizerState* tempState = 0;
	GetDeviceContext()->RSSetState(Resources()->RasterizerState);

	return 0;
}

HRESULT Renderer::CreateViewPort()
{
	GetDeviceContext()->RSSetViewports(1, Resources()->ViewPort);

	return 0;
}


HRESULT Renderer::SetVertexShader(LPCWSTR compiledShaderFile)
{
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = GetDevice()->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &Resources()->Shaders->VertexShader);

	if (FAILED(result))
	{
		return result;
	}

	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	return GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->FileBytes, vsByteCode->Length, &Resources()->InputLayout);
}

HRESULT Renderer::CompileVertexShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11VertexShader* vertexShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

HRESULT Renderer::SetHullShader(LPCWSTR compiledShaderFile)
{
	auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = Resources()->Shaders;
	return GetDevice()->CreateHullShader(hsByteCode->FileBytes, hsByteCode->Length, NULL, &shaders->HullShader);
}

HRESULT Renderer::CompileHullShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11HullShader* hullShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, HS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return GetDevice()->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &hullShader);
}

HRESULT Renderer::SetGeometryShader(LPCWSTR compiledShaderFile)
{
	auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = Resources()->Shaders;
	return GetDevice()->CreateGeometryShader(gsByteCode->FileBytes, gsByteCode->Length, NULL, &shaders->GeometryShader);
}

HRESULT Renderer::CompileGeometryShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11GeometryShader* geometryShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, GS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return GetDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &geometryShader);
}

HRESULT Renderer::SetPixelShader(LPCWSTR compiledShaderFile)
{
	auto psByteCode = File::ReadFileBytes(compiledShaderFile);
	return GetDevice()->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, NULL, &Resources()->Shaders->PixelShader);
}

HRESULT Renderer::CompilePixelShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11PixelShader* pixelShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, PS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
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

	return D3DCompileFromFile(compiledShaderFile,
		NULL, // Defines
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", // Entrypoint
		shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}

void Renderer::ClearScene()
{
	// Update the model data
	GetDeviceContext()->UpdateSubresource(Resources()->ConstBuffer, 0, nullptr, Resources()->ConstBufferData, 0, 0);

	// Clear render targets and depth stencil
	GetDeviceContext()->OMSetRenderTargets(1, &Resources()->RenderTargetView, Resources()->DepthStencilView);
	GetDeviceContext()->ClearRenderTargetView(Resources()->RenderTargetView, Resources()->DefaultColor);
	GetDeviceContext()->ClearDepthStencilView(Resources()->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

HRESULT Renderer::Render()
{
	// Clear
	ClearScene();

	if (!m_isInitialized)
	{
		return 0;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Set model data
	GetDeviceContext()->IASetInputLayout(Resources()->InputLayout);
	GetDeviceContext()->IASetVertexBuffers(0, 1, &Resources()->VertexBuffer, &stride, &offset);
	GetDeviceContext()->IASetIndexBuffer(Resources()->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shader data
	GetDeviceContext()->VSSetShader(Resources()->Shaders->VertexShader, nullptr, 0);
	GetDeviceContext()->VSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
	GetDeviceContext()->PSSetShader(Resources()->Shaders->PixelShader, nullptr, 0);

	GetDeviceContext()->DrawIndexed(Resources()->IndexCount, 0, 0);
	//GetDeviceContext()->Draw(Resources()->VertexCount, 0);

	// Present
	return Resources()->SwapChain->Present(1, 0);
}

HRESULT	Renderer::ResizeSwapChain(UINT32 newWidth, UINT32 newHeight)
{
	return Resources()->SwapChain->ResizeBuffers(Resources()->BufferCount, 0, 0, Resources()->RenderQuality->BufferFormat, Resources()->SwapChainFlags);
}

HRESULT Renderer::Resize(D3D11_VIEWPORT* viewPort)
{
	HRESULT result;
	Resources()->ViewPort = viewPort;
	if (NULL != Resources()->SwapChain && m_isInitialized)
	{
		result = Initialize();
	}
	return result;
}