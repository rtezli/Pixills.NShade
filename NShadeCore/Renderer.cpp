#include "stdafx.h"
#include "renderer.h"


Renderer::Renderer(bool useSwapChain)
{
    _isInitialized = false;
    _useSwapChain = useSwapChain;
    _rasterizerUseMultiSampling = true;
    Res::Get()->Shaders = new ShaderSet();
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

    //result = SetVertexShader(_standardVertexShader);
    //if (FAILED(result))
    //{
    //    return result;
    //}

    _isInitialized = true;
    return SetPixelShader(_standardPixelShader);
}


HRESULT Renderer::CreateRenderTargetDesciption()
{
    _renderTargetDesc.Width = Res::Get()->ViewPort->Width;
    _renderTargetDesc.Height = Res::Get()->ViewPort->Height;
    _renderTargetDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    _renderTargetDesc.ArraySize = 1;
    _renderTargetDesc.Format = Res::Get()->RenderQuality->TextureFormat;
    _renderTargetDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _renderTargetDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _renderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
    _renderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
    _renderTargetDesc.CPUAccessFlags = 0;
    _renderTargetDesc.MiscFlags = 0;

    ID3D11Texture2D* renderTarget = 0;

    return Res::Get()->Device->CreateTexture2D(&_renderTargetDesc, NULL, &renderTarget);
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

    return Res::Get()->Device->CreateRenderTargetView(Res::Get()->BackBuffer, &_renderTargetViewDesc, &Res::Get()->RenderTargetView);
}


HRESULT Renderer::CreateSwapChainDesciption()
{
    _swapChainDescription = { 0 };

    _swapChainDescription.BufferCount = Res::Get()->BufferCount;
    _swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    _swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    _swapChainDescription.Flags = Res::Get()->SwapChainFlags;

    _swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
    _swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    _swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    _swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    _swapChainDescription.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _swapChainDescription.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _swapChainDescription.BufferDesc.Format = Res::Get()->RenderQuality->TextureFormat;
    _swapChainDescription.BufferDesc.Width = Res::Get()->ViewPort->Width;
    _swapChainDescription.BufferDesc.Height = Res::Get()->ViewPort->Height;

    auto handle = *Res::Get()->WindowHandle;
    _swapChainDescription.OutputWindow = handle;

    if (Res::Get()->FullScreen)
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

    IDXGIDevice *dxgiDevice = 0;
    IDXGIAdapter *dxgiAdapter = 0;
    IDXGIFactory *dxgiFactory = 0;

    result = Res::Get()->Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
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

    result = dxgiFactory->CreateSwapChain(Res::Get()->Device, &_swapChainDescription, &Res::Get()->SwapChain);
    if (FAILED(result))
    {
        return result;
    }

    result = Res::Get()->SwapChain->GetBuffer(0, IID_PPV_ARGS(&Res::Get()->BackBuffer));
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
    _depthBufferDesc.Width = Res::Get()->ViewPort->Width;
    _depthBufferDesc.Height = Res::Get()->ViewPort->Height;
    _depthBufferDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    _depthBufferDesc.ArraySize = 1;
    _depthBufferDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    _depthBufferDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _depthBufferDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
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
    return Res::Get()->Device->CreateTexture2D(&_depthBufferDesc, NULL, &Res::Get()->DepthStencilBuffer);
}


HRESULT Renderer::CreateDepthStencilDescription()
{
    _depthStencilDesc.ArraySize = 1;
    _depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    _depthStencilDesc.MipLevels = Res::Get()->RenderQuality->MipLevels;
    _depthStencilDesc.SampleDesc.Quality = Res::Get()->RenderQuality->Quality;
    _depthStencilDesc.SampleDesc.Count = Res::Get()->RenderQuality->SampleCount;
    _depthStencilDesc.Format = Res::Get()->RenderQuality->BufferFormat;
    _depthStencilDesc.Width = Res::Get()->ViewPort->Width;
    _depthStencilDesc.Height = Res::Get()->ViewPort->Height;

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

    auto result = Res::Get()->Device->CreateDepthStencilState(&_depthStencilStateDesc, &Res::Get()->DepthStencilState);

    Res::Get()->DeviceContext->OMSetDepthStencilState(Res::Get()->DepthStencilState, 1);
    return result;
}

HRESULT Renderer::CreateDepthStencilViewDescription()
{
    _depthStencilViewDesc.Format = Res::Get()->RenderQuality->BufferFormat;
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

    result = Res::Get()->Device->CreateTexture2D(&_depthStencilDesc, NULL, &depthStencil);
    result = Res::Get()->Device->CreateDepthStencilView(depthStencil, &_depthStencilViewDesc, &Res::Get()->DepthStencilView);

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

    result = Res::Get()->Device->CreateRasterizerState(&_rasterizerDesc, &Res::Get()->RasterizerState);
    if (FAILED(result))
    {
        return result;
    }

    // Set rasterizer
    ID3D11RasterizerState* tempState = 0;
    Res::Get()->DeviceContext->RSSetState(Res::Get()->RasterizerState);

    return 0;
}

HRESULT Renderer::CreateViewPort()
{
    Res::Get()->DeviceContext->RSSetViewports(1, Res::Get()->ViewPort);

    return 0;
}


HRESULT Renderer::SetVertexShader(wchar_t *compiledShaderFile)
{
    auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
    auto result = Res::Get()->Device->CreateVertexShader(vsByteCode->Bytes, vsByteCode->Length, NULL, &Res::Get()->Shaders->VertexShader);

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

    return Res::Get()->Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->Bytes, vsByteCode->Length, &Res::Get()->InputLayout);
}

HRESULT Renderer::CompileVertexShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11VertexShader* vertexShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

HRESULT Renderer::SetHullShader(wchar_t *compiledShaderFile)
{
    auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
    auto shaders = Res::Get()->Shaders;
    return Res::Get()->Device->CreateHullShader(hsByteCode->Bytes, hsByteCode->Length, NULL, &shaders->HullShader);
}

HRESULT Renderer::CompileHullShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11HullShader* hullShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, HS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &hullShader);
}

HRESULT Renderer::SetGeometryShader(wchar_t *compiledShaderFile)
{
    auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
    auto shaders = Res::Get()->Shaders;
    return Res::Get()->Device->CreateGeometryShader(gsByteCode->Bytes, gsByteCode->Length, NULL, &shaders->GeometryShader);
}

HRESULT Renderer::CompileGeometryShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11GeometryShader* geometryShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, GS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &geometryShader);
}

HRESULT Renderer::SetPixelShader(wchar_t *compiledShaderFile)
{
    auto psByteCode = File::ReadFileBytes(compiledShaderFile);
    return Res::Get()->Device->CreatePixelShader(psByteCode->Bytes, psByteCode->Length, NULL, &Res::Get()->Shaders->PixelShader);
}

HRESULT Renderer::CompilePixelShader(wchar_t *compiledShaderFile)
{
    ID3DBlob* shaderBlob = 0;
    ID3D11PixelShader* pixelShader = 0;

    auto result = CompileShader(compiledShaderFile, shaderBlob, PS_PROFILE);
    if (FAILED(result))
    {
        return result;
    }
    return Res::Get()->Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
}

HRESULT Renderer::CompileShader(wchar_t *compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile)
{
    ID3DBlob* shaderBlob = 0;
    unsigned int flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

    const D3D_SHADER_MACRO defines[] =
    {
        { "Foo", "Bar" }
    };

    return D3DCompileFromFile(compiledShaderFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}


void Renderer::ClearScene()
{
    Res::Get()->DeviceContext->OMSetRenderTargets(1, &Res::Get()->RenderTargetView, Res::Get()->DepthStencilView);
    Res::Get()->DeviceContext->ClearRenderTargetView(Res::Get()->RenderTargetView, Res::Get()->DefaultColor);
    Res::Get()->DeviceContext->ClearDepthStencilView(Res::Get()->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

HRESULT Renderer::Render(Scene *scene)
{
    ClearScene();

    for (unsigned int i = 0; i < scene->GetModels()->size(); i++)
    {
        auto model = scene->GetModels()->at(i);
        auto material = model.GetMaterial();
        auto shaders = material->GetShaders();

        unsigned int offset = 0;

        auto vertexBuffer = model.GetVertexBuffer();
        auto strides = shaders->VertexShader->GetInputSize();
        auto indexBuffer = model.GetIndexBuffer();

        Res::Get()->DeviceContext->IASetInputLayout(shaders->VertexShader->GetInputLayout());
        Res::Get()->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
        Res::Get()->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        Res::Get()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Res::Get()->DeviceContext->VSSetShaderResources();
        // Res::Get()->DeviceContext->VSSetSamplers();
        auto cameraConstantBuffer = scene->GetCamera()->GetMatrixBuffer();
        auto materialColorBuffer = material->GetColorBuffer();
        auto ambientBuffer = scene->GetAmbientBuffer();

        //auto pointLightBuffer     = scene->GetLights()->GetAmbientBuffer();

        Res::Get()->DeviceContext->VSSetConstantBuffers(0, 1, &cameraConstantBuffer);
        Res::Get()->DeviceContext->VSSetConstantBuffers(1, 1, &materialColorBuffer);
        Res::Get()->DeviceContext->VSSetConstantBuffers(2, 1, &ambientBuffer);
        Res::Get()->DeviceContext->VSSetShader(shaders->VertexShader->GetShader(), NULL, 0);

        Res::Get()->DeviceContext->PSSetConstantBuffers(0, 1, &cameraConstantBuffer);
        Res::Get()->DeviceContext->PSSetShader(shaders->PixelShader->GetShader(), NULL, 0);

        Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }

    //unsigned int stride = sizeof(PhongShader::InputLayout);
    //unsigned int offset = 0;

    //Res::Get()->DeviceContext->IASetInputLayout(Res::Get()->InputLayout);
    //Res::Get()->DeviceContext->IASetVertexBuffers(0, 1, &Res::Get()->VertexBuffer, &stride, &offset);
    //Res::Get()->DeviceContext->IASetIndexBuffer(Res::Get()->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    //Res::Get()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //// Res::Get()->DeviceContext->VSSetShaderResources();
    //// Res::Get()->DeviceContext->VSSetSamplers();
    //auto cameraConstantBuffer = scene->GetCamera()->GetMatrixBuffer();
    //Res::Get()->DeviceContext->VSSetConstantBuffers(0, 1, &cameraConstantBuffer);
    //Res::Get()->DeviceContext->VSSetShader(Res::Get()->Shaders->VertexShader, NULL, 0);

    //Res::Get()->DeviceContext->PSSetConstantBuffers(0, 1, &cameraConstantBuffer);
    //Res::Get()->DeviceContext->PSSetShader(Res::Get()->Shaders->PixelShader, NULL, 0);

    //Res::Get()->DeviceContext->DrawIndexed(Res::Get()->IndexCount, 0, 0);

    return Res::Get()->SwapChain->Present(1, 0);
}

HRESULT	Renderer::ResizeSwapChain(UINT32 newWidth, UINT32 newHeight)
{
    return Res::Get()->SwapChain->ResizeBuffers(Res::Get()->BufferCount, 0, 0, Res::Get()->RenderQuality->BufferFormat, Res::Get()->SwapChainFlags);
}

HRESULT Renderer::Resize(D3D11_VIEWPORT* viewPort)
{
    HRESULT result;
    Res::Get()->ViewPort = viewPort;
    if (NULL != Res::Get()->SwapChain && _isInitialized)
    {
        result = Initialize();
    }
    return result;
}