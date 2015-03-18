#pragma once

#include "d3d11.h"
#include "directxmath.h"

using namespace DirectX;

struct NVertex
{
    XMFLOAT3 Position;
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
    DXGI_FORMAT     TextureFormat;
    DXGI_FORMAT     BufferFormat;
    unsigned int    Quality;
    unsigned int    SampleCount;
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
    XMFLOAT4    camera;
};

namespace ScreenRotation
{
    static const XMFLOAT4X4 Rotation0(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

    // 90-degree Z-rotation
    static const XMFLOAT4X4 Rotation90(
        0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

    // 180-degree Z-rotation
    static const XMFLOAT4X4 Rotation180(
        -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

    // 270-degree Z-rotation
    static const XMFLOAT4X4 Rotation270(
        0.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}
class Res
{
public:
    static Res* Get();
    ~Res(){}
    HINSTANCE                   *WindowInstance;
    HWND                        *WindowHandle;
    ID3D11Device                *Device;
    ID3D11DeviceContext         *DeviceContext;

    ID3D11RasterizerState       *RasterizerState;

    float                       *DefaultColor;
    D3D11_VIEWPORT              *ViewPort;
    RenderingQuality            *RenderQuality;

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
