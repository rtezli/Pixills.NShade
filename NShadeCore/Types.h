#pragma once

#include "windows.h"
#include "d3d11.h"

using namespace std;
using namespace DirectX;

enum Perspective : CHAR
{
    UNDEFINED = 0,
    FIRST_PERSON = 1,
    THIRD_PERSON = 2,
    ISOMETRIC = 3
};

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
    UINT P1;
    UINT P2;
    UINT P3;
};

enum LightType : CHAR
{
    Directional = 1,
    Point = 2,
    Spot = 3,
    Volume = 4,
    Area = 5,
    Ambient = 6
};

struct Light
{
    XMFLOAT4 Color;
    XMFLOAT4 Position;
};

struct VertexShaderInput
{
    NVertex Vertex;
    Light Light;
};


enum MSAA : CHAR
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

namespace Debug
{
    static VOID WriteLine(LPCWSTR message)
    {
#ifdef _DEBUG
        OutputDebugString(message);
#endif
    }

    static VOID WriteLine(const CHAR* m1, const CHAR* m2)
    {
#ifdef _DEBUG
        //auto mes = new wstring(message);
        //auto cstr = mes->c_str();
        //LPCWSTR str = cstr;
        //OutputDebugString(mes->c_str());
#endif
    }

    static VOID WriteLine(wstring message)
    {
#ifdef _DEBUG
        LPCWSTR str = message.c_str();
        OutputDebugString(str);
#endif
    }

    static VOID WriteLine(FLOAT message)
    {
#ifdef _DEBUG
        auto mes = to_wstring(message);
        mes.append(L"\n");
        LPCWSTR str = mes.c_str();
        OutputDebugString(str);
#endif
    }

    static VOID WriteLine(wstring m1, FLOAT m2)
    {
#ifdef _DEBUG
        auto mes = to_wstring(m2);
        m1.append(mes);
        m1.append(L"\n");
        LPCWSTR str = m1.c_str();
        OutputDebugString(str);
#endif
    }
    static VOID WriteCurrentDir()
    {
#ifdef _DEBUG
        wchar_t wtext[MAX_PATH];
        LPWSTR result = wtext;
        GetCurrentDirectory(MAX_PATH, result);
        OutputDebugString(result);
#endif
    }
}