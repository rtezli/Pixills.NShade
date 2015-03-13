#include "stdafx.h"
#include "renderer.h"


Renderer::Renderer(DeviceResources *resources, BOOL useSwapChain)
{
	_resources = resources;
	//Res::Get()->Device = resources->Device;
	//Res::Get()->DeviceContext = resources->DeviceContext;
	//Res::Get()->FullScreen = resources->FullScreen;
	//Res::Get()->RenderQuality = resources->RenderQuality;
	//Res::Get()->ViewPort = resources->ViewPort;
	//Res::Get()->WindowHandle = resources->WindowHandle;
	//Res::Get()->BufferCount = resources->BufferCount;
	//Res::Get()->BackBuffer = resources->BackBuffer;
	//Res::Get()->ConstBuffer = resources->ConstBuffer;
	//Res::Get()->ConstBufferData = resources->ConstBufferData;
	//Res::Get()->VertexBuffer = resources->VertexBuffer;
	//Res::Get()->VertexCount = resources->VertexCount;
	//Res::Get()->IndexBuffer = resources->IndexBuffer;
	//Res::Get()->IndexCount = resources->IndexCount;

	_isInitialized = false;
	//_deviceResources = resources;
	_resources->Shaders = new ShaderSet();
	_useSwapChain = useSwapChain;
	_rasterizerUseMultiSampling = true;

	DXGI_FORMAT swapChainBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT depthSencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT depthSencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DXGI_FORMAT vertexPositionFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	DXGI_FORMAT vertexColorFormat = DXGI_FORMAT_R32G32B32_FLOAT;
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

	result = SetVertexShader(_standardVertexShader);
	if (FAILED(result))
	{
		return result;
	}
	_isInitialized = true;
	return SetPixelShader(_standardPixelShader);
}


HRESULT Renderer::CreateRenderTargetDesciption()
{
	_renderTargetDesc.Width = _resources->ViewPort->Width;
	_renderTargetDesc.Height = _resources->ViewPort->Height;
	_renderTargetDesc.MipLevels = 1;
	_renderTargetDesc.ArraySize = 1;
	_renderTargetDesc.Format = _resources->RenderQuality->TextureFormat;
	_renderTargetDesc.SampleDesc.Quality = _resources->RenderQuality->Quality;
	_renderTargetDesc.SampleDesc.Count = _resources->RenderQuality->SampleCount;
	_renderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
	_renderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	_renderTargetDesc.CPUAccessFlags = 0;
	_renderTargetDesc.MiscFlags = 0;

	ID3D11Texture2D* renderTarget = 0;

	return _resources->Device->CreateTexture2D(&_renderTargetDesc, NULL, &renderTarget);
}

HRESULT Renderer::CreateRenderTargetViewDesciption()
{
	_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	_renderTargetViewDesc.Format = _renderTargetDesc.Format;
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

	return _resources->Device->CreateRenderTargetView(_resources->BackBuffer, &_renderTargetViewDesc, &_resources->RenderTargetView);
}


HRESULT Renderer::CreateSwapChainDesciption()
{
	_swapChainDescription = { 0 };

	_swapChainDescription.BufferCount = _resources->BufferCount;
	_swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	_swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	_swapChainDescription.Flags = _resources->SwapChainFlags;

	_swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	_swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	_swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	_swapChainDescription.SampleDesc.Quality = _resources->RenderQuality->Quality;
	_swapChainDescription.SampleDesc.Count = _resources->RenderQuality->SampleCount;
	_swapChainDescription.BufferDesc.Format = _resources->RenderQuality->TextureFormat;
	_swapChainDescription.BufferDesc.Width = _resources->ViewPort->Width;
	_swapChainDescription.BufferDesc.Height = _resources->ViewPort->Height;

	auto handle = *_resources->WindowHandle;
	_swapChainDescription.OutputWindow = handle;

	if (_resources->FullScreen)
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

	result = _resources->Device->QueryInterface(__uuidof(IDXGIDevice), (VOID **)&dxgiDevice);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (VOID **)&dxgiAdapter);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (VOID **)&dxgiFactory);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiFactory->CreateSwapChain(_resources->Device, &_swapChainDescription, &_resources->SwapChain);
	if (FAILED(result))
	{
		return result;
	}

	result = _resources->SwapChain->GetBuffer(0, IID_PPV_ARGS(&_resources->BackBuffer));
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
	_depthBufferDesc.Width = _resources->ViewPort->Width;
	_depthBufferDesc.Height = _resources->ViewPort->Height;
	_depthBufferDesc.MipLevels = 1;
	_depthBufferDesc.ArraySize = 1;
	_depthBufferDesc.Format = _resources->RenderQuality->BufferFormat;
	_depthBufferDesc.SampleDesc.Quality = _resources->RenderQuality->Quality;
	_depthBufferDesc.SampleDesc.Count = _resources->RenderQuality->SampleCount;
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
	return _resources->Device->CreateTexture2D(&_depthBufferDesc, NULL, &_resources->DepthStencilBuffer);
}


HRESULT Renderer::CreateDepthStencilDescription()
{
	_depthStencilDesc.ArraySize = 1;
	_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthStencilDesc.MipLevels = _resources->RenderQuality->Quality > 0 ? 1 : 0;
	_depthStencilDesc.SampleDesc.Quality = _resources->RenderQuality->Quality;
	_depthStencilDesc.SampleDesc.Count = _resources->RenderQuality->SampleCount;
	_depthStencilDesc.Format = _resources->RenderQuality->BufferFormat;
	_depthStencilDesc.Width = _resources->ViewPort->Width;
	_depthStencilDesc.Height = _resources->ViewPort->Height;

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

	auto result = _resources->Device->CreateDepthStencilState(&_depthStencilStateDesc, &_resources->DepthStencilState);

	_resources->DeviceContext->OMSetDepthStencilState(_resources->DepthStencilState, 1);
	return result;
}

HRESULT Renderer::CreateDepthStencilViewDescription()
{
	_depthStencilViewDesc.Format = _resources->RenderQuality->BufferFormat;
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

	result = _resources->Device->CreateTexture2D(&_depthStencilDesc, NULL, &depthStencil);
	result = _resources->Device->CreateDepthStencilView(depthStencil, &_depthStencilViewDesc, &_resources->DepthStencilView);

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
	//_rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

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

	result = _resources->Device->CreateRasterizerState(&_rasterizerDesc, &_resources->RasterizerState);
	if (FAILED(result))
	{
		return result;
	}

	// Set rasterizer
	ID3D11RasterizerState* tempState = 0;
	_resources->DeviceContext->RSSetState(_resources->RasterizerState);

	return 0;
}

HRESULT Renderer::CreateViewPort()
{
	_resources->DeviceContext->RSSetViewports(1, _resources->ViewPort);

	return 0;
}


HRESULT Renderer::SetVertexShader(LPCWSTR compiledShaderFile)
{
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = _resources->Device->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &_resources->Shaders->VertexShader);

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

	return _resources->Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->FileBytes, vsByteCode->Length, &_resources->InputLayout);
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
	return _resources->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

HRESULT Renderer::SetHullShader(LPCWSTR compiledShaderFile)
{
	auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = _resources->Shaders;
	return _resources->Device->CreateHullShader(hsByteCode->FileBytes, hsByteCode->Length, NULL, &shaders->HullShader);
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
	return _resources->Device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &hullShader);
}

HRESULT Renderer::SetGeometryShader(LPCWSTR compiledShaderFile)
{
	auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = _resources->Shaders;
	return _resources->Device->CreateGeometryShader(gsByteCode->FileBytes, gsByteCode->Length, NULL, &shaders->GeometryShader);
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
	return _resources->Device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &geometryShader);
}

HRESULT Renderer::SetPixelShader(LPCWSTR compiledShaderFile)
{
	auto psByteCode = File::ReadFileBytes(compiledShaderFile);
	return _resources->Device->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, NULL, &_resources->Shaders->PixelShader);
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
	return _resources->Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
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
		{"Foo", "Bar" }
	};

	return D3DCompileFromFile(compiledShaderFile,defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}

VOID Renderer::ClearScene()
{
	// Update the model data
	_resources->DeviceContext->UpdateSubresource(_resources->ConstBuffer, 0, NULL, _resources->ConstBufferData, 0, 0);

	// Clear render targets and depth stencil
	_resources->DeviceContext->OMSetRenderTargets(1, &_resources->RenderTargetView, _resources->DepthStencilView);
	_resources->DeviceContext->ClearRenderTargetView(_resources->RenderTargetView, _resources->DefaultColor);
	_resources->DeviceContext->ClearDepthStencilView(_resources->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

HRESULT Renderer::Render()
{
	// Clear
	ClearScene();

	if (!_isInitialized)
	{
		return 0;
	}

	UINT stride = sizeof(PhongShader::InputLayout);
	UINT offset = 0;

	// Set model data
	_resources->DeviceContext->IASetInputLayout(_resources->InputLayout);
	
	// Set multiple buffers here ? i.e. each for one model since some shaders are not applied to all models
	_resources->DeviceContext->IASetVertexBuffers(0, 1, &_resources->VertexBuffer, &stride, &offset);
	_resources->DeviceContext->IASetIndexBuffer(_resources->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_resources->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set shader data
	

	// Set multiple shaders here ?
	auto vs = _resources->Shaders->VertexShader;
	auto ps = _resources->Shaders->PixelShader;

	_resources->DeviceContext->VSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
	_resources->DeviceContext->VSSetShader(vs, NULL, 0);
	
	_resources->DeviceContext->VSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
	_resources->DeviceContext->VSSetShader(vs, NULL, 0);

	_resources->DeviceContext->PSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
	_resources->DeviceContext->PSSetShader(ps, NULL, 0);

	_resources->DeviceContext->DrawIndexed(_resources->IndexCount, 0, 0);

	// Present
	return _resources->SwapChain->Present(1, 0);
}

HRESULT	Renderer::ResizeSwapChain(UINT32 newWidth, UINT32 newHeight)
{
	return _resources->SwapChain->ResizeBuffers(_resources->BufferCount, 0, 0, _resources->RenderQuality->BufferFormat, _resources->SwapChainFlags);
}

HRESULT Renderer::Resize(D3D11_VIEWPORT* viewPort)
{
	HRESULT result;
	_resources->ViewPort = viewPort;
	if (NULL != _resources->SwapChain && _isInitialized)
	{
		result = Initialize();
	}
	return result;
}