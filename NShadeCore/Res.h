#pragma once

#include "d3d11.h"
#include "directxmath.h"

using namespace DirectX;

struct NVertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT3 Normal;
    XMFLOAT2 UV;
    XMFLOAT3 PolyPosition;
};

struct NPolygon
{
    unsigned int P1;
    unsigned int P2;
    unsigned int P3;
};

struct RenderingQuality
{
    unsigned int    Quality;
    unsigned int    SampleCount;
    DXGI_FORMAT     TextureFormat;
    DXGI_FORMAT     BufferFormat;
    unsigned int    MipLevels;
};

struct ResourceSettings
{
    RECT                Size;
    RenderingQuality    Quality;
};

enum MSAA : char
{
    SIMPLEST_POSSIBLE = 9,
    MSAA_0X = 0,
    MSAA_1X = 1,
    MSAA_2X = 2,
    MSAA_4X = 4,
    MSAA_8X = 8,
    BEST_POSSIBLE = 8
};

struct MsaaOptions
{
    MSAA Msaa;
    RenderingQuality Quality;
};

struct ConstantBufferData
{
    XMFLOAT4X4  world;
    XMFLOAT4X4  view;
    XMFLOAT4X4  projection;
    XMFLOAT3    camera;
    float       time;
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
    float                       *DefaultColor;
    D3D11_VIEWPORT              *ViewPort;
    RenderingQuality            *RenderQuality;

    int                         SwapChainFlags;
    int                         BufferCount;
    int                         IndexCount;
    int                         VertexCount;

    float                       Dpi;
    float                       NearZ;
    float                       FarZ;

    bool                        FullScreen;
    bool                        VSync;
private:
    static Res *instance;
    Res(){}
    Res(const Res&);
    Res & operator = (const Res &);
};
