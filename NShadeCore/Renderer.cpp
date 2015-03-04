#include "stdafx.h"

#include "renderer.h"


Renderer::Renderer(DeviceResources* pResources, bool useSwapChain)
{
	m_isInitialized = false;
	m_pDeviceResources = shared_ptr<DeviceResources>(pResources);

	m_useSwapChain = useSwapChain;
	m_renderShadows = true;
	m_rasterizerUseMultiSampling = true;

	// TODO : Get available formats dynamically
	// 8 bit per channel (rgba)
	RenderingQuality quality0_8 = { 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, false }; // Should work for everyone
	RenderingQuality quality2_8 = { 2, 2, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality4_8 = { 16, 4, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality8_8 = { 32, 8, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true }; // Works on my machine ;)

	// 16 bit per channel  (rgba)
	RenderingQuality quality0_16 = { 0, 1, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_D32_FLOAT, true };

	m_pRenderingQuality = new RenderingQuality(quality4_8);
}

Renderer::~Renderer()
{

}

HRESULT Renderer::Initialize(Scene* scene)
{
	m_pScene = shared_ptr<Scene>(scene);
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

	//result = SetVertexShader(m_standardVertexShader);
	//if (FAILED(result))
	//{
	//	return result;
	//}

	result = CreateShadows();
	if (FAILED(result))
	{
		return result;
	}

	result = CreateLights();
	if (FAILED(result))
	{
		return result;
	}

	m_isInitialized = true;
	return result;
}


HRESULT Renderer::CreateRenderTargetDesciption()
{
	m_pRenderTargetDesc.Width = GetResources()->ViewPort->Width;
	m_pRenderTargetDesc.Height = GetResources()->ViewPort->Height;
	m_pRenderTargetDesc.MipLevels = 1;
	m_pRenderTargetDesc.ArraySize = 1;
	m_pRenderTargetDesc.Format = m_pRenderingQuality->TextureFormat;
	m_pRenderTargetDesc.SampleDesc.Quality = m_pRenderingQuality->Quality;
	m_pRenderTargetDesc.SampleDesc.Count = m_pRenderingQuality->SampleCount;
	m_pRenderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
	m_pRenderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	m_pRenderTargetDesc.CPUAccessFlags = 0;
	m_pRenderTargetDesc.MiscFlags = 0;

	ID3D11Texture2D* renderTarget = 0;

	return GetDevice()->CreateTexture2D(&m_pRenderTargetDesc, nullptr, &renderTarget);
}

HRESULT Renderer::CreateRenderTargetViewDesciption()
{
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

	return GetDevice()->CreateRenderTargetView(GetResources()->BackBuffer, &m_pRenderTargetViewDesc, &GetResources()->RenderTargetView);
}


HRESULT Renderer::CreateSwapChainDesciption()
{
	m_pSwapChainDescription = { 0 };

	m_pSwapChainDescription.BufferCount = GetResources()->BufferCount;
	m_pSwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	m_pSwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_pSwapChainDescription.Flags = GetResources()->SwapChainFlags;

	m_pSwapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	m_pSwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	m_pSwapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_pSwapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	m_pSwapChainDescription.SampleDesc.Quality = m_pRenderingQuality->Quality;
	m_pSwapChainDescription.SampleDesc.Count = m_pRenderingQuality->SampleCount;
	m_pSwapChainDescription.BufferDesc.Format = m_pRenderingQuality->TextureFormat;
	m_pSwapChainDescription.BufferDesc.Width = GetResources()->ViewPort->Width;
	m_pSwapChainDescription.BufferDesc.Height = GetResources()->ViewPort->Height;

	auto handle = *GetResources()->WindowHandle;
	m_pSwapChainDescription.OutputWindow = handle;

	if (GetResources()->FullScreen)
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

	result = dxgiFactory->CreateSwapChain(GetDevice(), &m_pSwapChainDescription, &GetResources()->SwapChain);
	if (FAILED(result))
	{
		return result;
	}

	result = GetResources()->SwapChain->GetBuffer(0, IID_PPV_ARGS(&GetResources()->BackBuffer));
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
	m_pDepthBufferDesc.Width = GetResources()->ViewPort->Width;
	m_pDepthBufferDesc.Height = GetResources()->ViewPort->Height;
	m_pDepthBufferDesc.MipLevels = 1;
	m_pDepthBufferDesc.ArraySize = 1;
	m_pDepthBufferDesc.Format = m_pRenderingQuality->BufferFormat;
	m_pDepthBufferDesc.SampleDesc.Quality = m_pRenderingQuality->Quality;
	m_pDepthBufferDesc.SampleDesc.Count = m_pRenderingQuality->SampleCount;
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
	return GetDevice()->CreateTexture2D(&m_pDepthBufferDesc, nullptr, &GetResources()->DepthStencilBuffer);
}


HRESULT Renderer::CreateDepthStencilDescription()
{
	m_pDepthStencilDesc.ArraySize = 1;
	m_pDepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_pDepthStencilDesc.MipLevels = m_pRenderingQuality->Quality > 0 ? 1 : 0;
	m_pDepthStencilDesc.SampleDesc.Quality = m_pRenderingQuality->Quality;
	m_pDepthStencilDesc.SampleDesc.Count = m_pRenderingQuality->SampleCount;
	m_pDepthStencilDesc.Format = m_pRenderingQuality->BufferFormat;
	m_pDepthStencilDesc.Width = GetResources()->ViewPort->Width;
	m_pDepthStencilDesc.Height = GetResources()->ViewPort->Height;

	return 0;
}

HRESULT Renderer::CreateDepthStencilStateDescription()
{
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

	auto result = GetDevice()->CreateDepthStencilState(&m_pDepthStencilStateDesc, &GetResources()->DepthStencilState);

	GetDeviceContext()->OMSetDepthStencilState(GetResources()->DepthStencilState, 1);
	return result;
}

HRESULT Renderer::CreateDepthStencilViewDescription()
{
	m_pDepthStencilViewDesc.Format = m_pRenderingQuality->BufferFormat;
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

	result = GetDevice()->CreateTexture2D(&m_pDepthStencilDesc, nullptr, &depthStencil);
	result = GetDevice()->CreateDepthStencilView(depthStencil, &m_pDepthStencilViewDesc, &GetResources()->DepthStencilView);

	if (FAILED(result))
	{
		return result;
	}
	return 0;
}


HRESULT Renderer::CreateRasterizerDescription()
{
	auto isRightHand = false;

	m_pRasterizerDesc.AntialiasedLineEnable = m_rasterizerUseMultiSampling;
	m_pRasterizerDesc.DepthBias = 0;
	m_pRasterizerDesc.DepthBiasClamp = 0.0f;
	m_pRasterizerDesc.DepthClipEnable = true;
	m_pRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//m_pRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	m_pRasterizerDesc.MultisampleEnable = m_rasterizerUseMultiSampling;
	m_pRasterizerDesc.ScissorEnable = false;
	m_pRasterizerDesc.SlopeScaledDepthBias = 0.0f;

	m_pRasterizerDesc.CullMode = D3D11_CULL_NONE;// D3D11_CULL_BACK;//
	m_pRasterizerDesc.FrontCounterClockwise = true;

	return 0;
}

HRESULT Renderer::CreateRasterizer()
{
	auto result = CreateRasterizerDescription();
	if (FAILED(result))
	{
		return result;
	}

	result = GetDevice()->CreateRasterizerState(&m_pRasterizerDesc, &GetResources()->RasterizerState);
	if (FAILED(result))
	{
		return result;
	}

	// Set rasterizer
	ID3D11RasterizerState* tempState = 0;
	GetDeviceContext()->RSSetState(GetResources()->RasterizerState);

	return 0;
}

HRESULT Renderer::CreateViewPort()
{
	GetDeviceContext()->RSSetViewports(1, GetResources()->ViewPort);

	return 0;
}

HRESULT Renderer::CreateShadows()
{
	auto textureDesc = m_pDepthStencilDesc;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	auto result = GetResources()->Device->CreateTexture2D(&textureDesc, NULL, &GetResources()->ShadowTexture);
	if (FAILED(result))
	{
		return result;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

	result = GetResources()->Device->CreateRenderTargetView(GetResources()->ShadowTexture, &renderTargetViewDesc, &GetResources()->ShadowRenderTarget);
	if (FAILED(result))
	{
		return result;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	result = GetResources()->Device->CreateShaderResourceView(GetResources()->ShadowTexture, &shaderResourceViewDesc, &GetResources()->ShadowResourceView);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Renderer::CreateLights()
{
	return 0;
}

HRESULT Renderer::Render()
{
	GetScene()->Clear();
	GetScene()->Render();
 
	GetDeviceContext()->DrawIndexed(GetResources()->IndexCount, 0, 0);

	return GetResources()->SwapChain->Present(1, 0);
}

HRESULT	Renderer::ResizeSwapChain(UINT32 newWidth, UINT32 newHeight)
{
	return GetResources()->SwapChain->ResizeBuffers(GetResources()->BufferCount, 0, 0, m_pRenderingQuality->BufferFormat, GetResources()->SwapChainFlags);
}

HRESULT Renderer::Resize(D3D11_VIEWPORT* viewPort)
{
	HRESULT result;
	GetResources()->ViewPort = viewPort;
	if (nullptr != GetResources()->SwapChain && m_isInitialized)
	{
		result = Initialize(GetScene());
	}
	return result;
}

/*
HRESULT Renderer::SetVertexShader(LPCWSTR compiledShaderFile)
{
auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
auto result = GetDevice()->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, nullptr, &GetResources()->Shaders->VertexShader);

if (FAILED(result))
{
return result;
}

static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
{
{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex position
{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Model color
{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Normal vector

{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Ambient light color where w is intensity
{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }  // Point light position w is intensity
};

return GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->FileBytes, vsByteCode->Length, &GetResources()->InputLayout);
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
return GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader);
}

HRESULT Renderer::SetHullShader(LPCWSTR compiledShaderFile)
{
auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
auto shaders = GetResources()->Shaders;
return GetDevice()->CreateHullShader(hsByteCode->FileBytes, hsByteCode->Length, nullptr, &shaders->HullShader);
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
return GetDevice()->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &hullShader);
}

HRESULT Renderer::SetGeometryShader(LPCWSTR compiledShaderFile)
{
auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
auto shaders = GetResources()->Shaders;
return GetDevice()->CreateGeometryShader(gsByteCode->FileBytes, gsByteCode->Length, nullptr, &shaders->GeometryShader);
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
return GetDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &geometryShader);
}

HRESULT Renderer::SetPixelShader(LPCWSTR compiledShaderFile)
{
auto psByteCode = File::ReadFileBytes(compiledShaderFile);
return GetDevice()->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, nullptr, &GetResources()->Shaders->PixelShader);
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
return GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader);
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
{ "Foo", "Bar" }
};

return D3DCompileFromFile(compiledShaderFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}
*/