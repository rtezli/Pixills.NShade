#include "stdafx.h"

#include "renderer.h"


Renderer::Renderer(DeviceResources *pResources, BOOL useSwapChain)
{
	_deviceResources = shared_ptr<DeviceResources>(pResources);

	_useSwapChain = useSwapChain;
	_renderShadows = true;
	_rasterizerUseMultiSampling = true;

	// TODO : Get available formats dynamically
	// 8 bit per channel (rgba)
	RenderingQuality quality0_8 = { 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, false }; // Should work for everyone
	RenderingQuality quality2_8 = { 2, 2, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality4_8 = { 16, 4, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true };
	RenderingQuality quality8_8 = { 32, 8, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, true }; // Works on my machine ;)

	// 16 bit per channel  (rgba)
	RenderingQuality quality0_16 = { 0, 1, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_D32_FLOAT, true };

	_renderingQuality = new RenderingQuality(quality4_8);
}

HRESULT Renderer::Initialize(Scene *scene)
{
	_scene = shared_ptr<Scene>(scene);
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

	//result = CreateShadows();
	//if (FAILED(result))
	//{
	//	return result;
	//}

	return result;
}


HRESULT Renderer::CreateRenderTargetDesciption()
{
	_renderTargetDesc.Width = GetResources()->ViewPort->Width;
	_renderTargetDesc.Height = GetResources()->ViewPort->Height;
	_renderTargetDesc.MipLevels = 1;
	_renderTargetDesc.ArraySize = 1;
	_renderTargetDesc.Format = _renderingQuality->TextureFormat;
	_renderTargetDesc.SampleDesc.Quality = _renderingQuality->Quality;
	_renderTargetDesc.SampleDesc.Count = _renderingQuality->SampleCount;
	_renderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
	_renderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	_renderTargetDesc.CPUAccessFlags = 0;
	_renderTargetDesc.MiscFlags = 0;

	ID3D11Texture2D* renderTarget = 0;

	return GetDevice()->CreateTexture2D(&_renderTargetDesc, NULL, &renderTarget);
}

HRESULT Renderer::CreateRenderTargetViewDesciption()
{
	_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	_renderTargetViewDesc.Format = _renderingQuality->TextureFormat;
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

	return GetDevice()->CreateRenderTargetView(GetResources()->BackBuffer, &_renderTargetViewDesc, &GetResources()->RenderTargetView);
}


HRESULT Renderer::CreateSwapChainDesciption()
{
	_swapChainDescription = { 0 };

	_swapChainDescription.BufferCount = GetResources()->BufferCount;
	_swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	_swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	_swapChainDescription.Flags = GetResources()->SwapChainFlags;

	_swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	_swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	_swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	_swapChainDescription.SampleDesc.Quality = _renderingQuality->Quality;
	_swapChainDescription.SampleDesc.Count = _renderingQuality->SampleCount;
	_swapChainDescription.BufferDesc.Format = _renderingQuality->TextureFormat;
	_swapChainDescription.BufferDesc.Width = GetResources()->ViewPort->Width;
	_swapChainDescription.BufferDesc.Height = GetResources()->ViewPort->Height;

	auto handle = *GetResources()->WindowHandle;
	_swapChainDescription.OutputWindow = handle;

	if (GetResources()->FullScreen)
	{
		_swapChainDescription.Windowed = false;
	}
	else
	{
		_swapChainDescription.Windowed = true;
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

	result = dxgiFactory->CreateSwapChain(GetDevice(), &_swapChainDescription, &GetResources()->SwapChain);
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
	_depthBufferDesc.Width = GetResources()->ViewPort->Width;
	_depthBufferDesc.Height = GetResources()->ViewPort->Height;
	_depthBufferDesc.MipLevels = 1;
	_depthBufferDesc.ArraySize = 1;
	_depthBufferDesc.Format = _renderingQuality->BufferFormat;
	_depthBufferDesc.SampleDesc.Quality = _renderingQuality->Quality;
	_depthBufferDesc.SampleDesc.Count = _renderingQuality->SampleCount;
	_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthBufferDesc.CPUAccessFlags = 0;
	_depthBufferDesc.MiscFlags = 0;

	return 0;
}

HRESULT Renderer::CreateDepthBuffer()
{
	auto result = CreateDepthBufferDescription();
	if (FAILED(result))
	{
		return result;
	}
	return GetDevice()->CreateTexture2D(&_depthBufferDesc, NULL, &GetResources()->DepthStencilBuffer);
}


HRESULT Renderer::CreateDepthStencilDescription()
{
	_depthStencilDesc.ArraySize = 1;
	_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthStencilDesc.MipLevels = _renderingQuality->Quality > 0 ? 1 : 0;
	_depthStencilDesc.SampleDesc.Quality = _renderingQuality->Quality;
	_depthStencilDesc.SampleDesc.Count = _renderingQuality->SampleCount;
	_depthStencilDesc.Format = _renderingQuality->BufferFormat;
	_depthStencilDesc.Width = GetResources()->ViewPort->Width;
	_depthStencilDesc.Height = GetResources()->ViewPort->Height;

	return 0;
}

HRESULT Renderer::CreateDepthStencilStateDescription()
{
	_depthStencilStateDesc.DepthEnable = true;
	_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	_depthStencilStateDesc.StencilEnable = true;
	_depthStencilStateDesc.StencilReadMask = 0xFF;
	_depthStencilStateDesc.StencilWriteMask = 0xFF;

	_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	auto result = GetDevice()->CreateDepthStencilState(&_depthStencilStateDesc, &GetResources()->DepthStencilState);

	GetDeviceContext()->OMSetDepthStencilState(GetResources()->DepthStencilState, 1);
	return result;
}

HRESULT Renderer::CreateDepthStencilViewDescription()
{
	_depthStencilViewDesc.Format = _renderingQuality->BufferFormat;
	_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	_depthStencilViewDesc.Texture2D.MipSlice = 0;

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

	result = GetDevice()->CreateTexture2D(&_depthStencilDesc, NULL, &depthStencil);
	result = GetDevice()->CreateDepthStencilView(depthStencil, &_depthStencilViewDesc, &GetResources()->DepthStencilView);

	if (FAILED(result))
	{
		return result;
	}
	return 0;
}


HRESULT Renderer::CreateRasterizerDescription()
{
	auto isRightHand = false;

	_rasterizerDesc.AntialiasedLineEnable = _rasterizerUseMultiSampling;
	_rasterizerDesc.DepthBias = 0;
	_rasterizerDesc.DepthBiasClamp = 0.0f;
	_rasterizerDesc.DepthClipEnable = true;
	_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//m_pRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	_rasterizerDesc.MultisampleEnable = _rasterizerUseMultiSampling;
	_rasterizerDesc.ScissorEnable = false;
	_rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	_rasterizerDesc.CullMode = D3D11_CULL_NONE;// D3D11_CULL_BACK;//
	_rasterizerDesc.FrontCounterClockwise = true;

	return 0;
}

HRESULT Renderer::CreateRasterizer()
{
	auto result = CreateRasterizerDescription();
	if (FAILED(result))
	{
		return result;
	}

	result = GetDevice()->CreateRasterizerState(&_rasterizerDesc, &GetResources()->RasterizerState);
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
	auto textureDesc = _depthStencilDesc;
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


HRESULT Renderer::Render()
{
	const FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetDeviceContext()->OMSetRenderTargets(1, &GetResources()->RenderTargetView, GetResources()->DepthStencilView);
	GetDeviceContext()->ClearRenderTargetView(GetResources()->RenderTargetView, color);
	GetDeviceContext()->ClearDepthStencilView(GetResources()->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	GetScene()->Clear();
	GetScene()->Render();

	return GetResources()->SwapChain->Present(1, 0);
}

HRESULT	Renderer::ResizeSwapChain(UINT32 newWidth, UINT32 newHeight)
{
	return GetResources()->SwapChain->ResizeBuffers(GetResources()->BufferCount, 0, 0, _renderingQuality->BufferFormat, GetResources()->SwapChainFlags);
}

HRESULT Renderer::Resize(D3D11_VIEWPORT *viewPort)
{
	HRESULT result;
	GetResources()->ViewPort = viewPort;
	if (NULL != GetResources()->SwapChain)
	{
		result = Initialize(GetScene());
	}
	return result;
}