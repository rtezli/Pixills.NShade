#include "stdafx.h"
#include "renderer.h"

Renderer::Renderer(DeviceResources* pResources)
{
	m_pDeviceResources = shared_ptr<DeviceResources>(pResources);
}

Renderer::~Renderer()
{
	m_pRenderTarget->Release();
	m_pDepthStencilView->Release();
}

HRESULT Renderer::Initialize()
{
	CreateSwapChain();

	auto result = SetVertexShader(m_standardVertexShader);
	if (FAILED(result))
	{
		return result;
	}

	result = SetPixelShader(m_standardPixelShader);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Renderer::CreateSwapChain()
{
	IDXGIDevice* dxgiDevice = 0;
	IDXGIAdapter* dxgiAdapter = 0;
	IDXGIFactory1* dxgiFactory = 0;

 
	auto result = GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);

	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter);
	if (FAILED(result))
	{
		return result;
	}

	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), (void **)&dxgiFactory);
	if (FAILED(result))
	{
		return result;
	}

	m_pDXGIDevice  = std::shared_ptr<IDXGIDevice>(dxgiDevice);
	m_pDXGIAdapter = std::shared_ptr<IDXGIAdapter>(dxgiAdapter);
	m_pDXGIFactory = std::shared_ptr<IDXGIFactory1>(dxgiFactory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;

	swapChainDesc.BufferDesc.Width = DeviceResource()->ScreenWidth;
	swapChainDesc.BufferDesc.Height = DeviceResource()->ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SampleDesc.Count = DeviceResource()->SamplesCount;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	auto handle = *DeviceResource()->WindowHandle;
	swapChainDesc.OutputWindow = handle;

	if (DeviceResource()->FullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	IDXGISwapChain* swapChain;
	result = m_pDXGIFactory->CreateSwapChain(GetDevice(), &swapChainDesc, &swapChain);
	if (FAILED(result))
	{
		return result;
	}
	m_pSwapChain = std::shared_ptr<IDXGISwapChain>(swapChain);
	return result;
}

HRESULT Renderer::SetVertexShader(LPCWSTR compiledShaderFile)
{
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = DeviceResource()->Shaders;

	auto result = GetDevice()->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &shaders->VertexShader);
	if (FAILED(result))
	{
		return result;
	}

	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D11InputLayout* inputLayout;
	result = GetDevice()->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsByteCode->FileBytes,
		vsByteCode->Length,
		&inputLayout);

	if (FAILED(result))
	{
		return result;
	}

	m_pInputLayout = std::shared_ptr<ID3D11InputLayout>(inputLayout);

	return result;
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
	auto shaders = DeviceResource()->Shaders;
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
	auto shaders = DeviceResource()->Shaders;
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
	auto shaders = DeviceResource()->Shaders;

	return GetDevice()->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, NULL, &shaders->PixelShader);
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
	//SetBuffers();

	ID3D11Texture2D* backBuffer = 0;
	ID3D11RenderTargetView* targetView = 0;

	auto swapChain = m_pSwapChain.get();
	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	GetDevice()->CreateRenderTargetView(backBuffer, nullptr, &targetView);
	swapChain->Present(1, 0);
}

void Renderer::SetBuffers()
{
	auto context = DeviceResource()->DeviceContext;
	auto constBuffer = DeviceResource()->ConstantBuffer;
	auto bufferData = DeviceResource()->ConstantBufferData;

	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;

	context->UpdateSubresource(constBuffer, 0, NULL, bufferData, 0, 0);
	context->IASetVertexBuffers(0, 1, &DeviceResource()->VertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(DeviceResource()->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_pInputLayout.get());
	context->VSSetShader(DeviceResource()->Shaders->VertexShader, nullptr, 0);
	context->PSSetShader(DeviceResource()->Shaders->PixelShader, nullptr, 0);
	context->DrawIndexed(DeviceResource()->IndexCount, 0, 0);
}