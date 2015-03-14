#pragma once

#include "d3d11.h"
#include "directxmath.h"

using namespace DirectX;

struct RenderingQuality
{
    UINT        Quality;
    UINT        SampleCount;
    DXGI_FORMAT TextureFormat;
    DXGI_FORMAT BufferFormat;
    UINT        MipLevels;
};

struct ResourceSettings
{
    RECT                Size;
    RenderingQuality    Quality;
};

struct ConstantBufferData
{
    XMFLOAT4X4  world;
    XMFLOAT4X4  view;
    XMFLOAT4X4  projection;
    XMFLOAT3    camera;
    FLOAT       time;
};

struct ShaderSet
{
    ID3D11PixelShader      *PixelShader;
    ID3D11VertexShader     *VertexShader;
    ID3D11HullShader       *HullShader;
    ID3D11DomainShader     *DomainShader;
    ID3D11GeometryShader   *GeometryShader;
    ID3D11ComputeShader    *ComputeShader;
};

class Res
{
public:
    static Res* Get();
    ~Res(){}
    HINSTANCE                   *WindowInstance;
    HWND                        *WindowHandle;
    ID3D11Device                *Device;
    ID3D11DeviceContext         *DeviceContext;

    ID3D11Buffer                *VertexBuffer;
    ID3D11Buffer                *IndexBuffer;
    ID3D11Buffer                *ConstBuffer;
    ConstantBufferData          *ConstBufferData;

    ID3D11Texture2D             *BackBuffer;
    ID3D11Texture2D             *DepthStencilBuffer;

    ID3D11DepthStencilState     *DepthStencilState;
    ID3D11RasterizerState       *RasterizerState;

    IDXGISwapChain              *SwapChain;

    ID3D11RenderTargetView      *RenderTargetView;
    ID3D11DepthStencilView      *DepthStencilView;
    ID3D11InputLayout           *InputLayout;

    ShaderSet                   *Shaders;
    FLOAT                       *DefaultColor;
    D3D11_VIEWPORT              *ViewPort;
    RenderingQuality            *RenderQuality;

    INT                         SwapChainFlags;
    INT                         BufferCount;
    INT                         IndexCount;
    INT                         VertexCount;

    FLOAT                       Dpi;
    FLOAT                       NearZ;
    FLOAT                       FarZ;

    BOOL                        FullScreen;
    BOOL                        VSync;
private:
    static Res *instance;
    Res(){}
    Res(const Res&);
    Res & operator = (const Res &);
};
