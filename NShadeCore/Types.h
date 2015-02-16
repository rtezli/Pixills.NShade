#pragma once

#include "windows.h"
#include "d3d11.h"

using namespace std;
using namespace DirectX;

enum Perspective : char
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Color;
	XMFLOAT3 UV;
	XMFLOAT3 Normal;
};

struct RenderingQuality
{
	UINT Quality;
	UINT SampleCount;
	DXGI_FORMAT TextureFormat;
	DXGI_FORMAT BufferFormat;
	bool IsMultisamplingSettings;
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
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct ShaderSet
{
	ID3D11PixelShader*		PixelShader;
	ID3D11VertexShader*		VertexShader;
	ID3D11HullShader*		HullShader;
	ID3D11DomainShader*		DomainShader;
	ID3D11GeometryShader*	GeometryShader;
	ID3D11ComputeShader*	ComputeShader;
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
	static void WriteLine(LPCWSTR message)
	{
#ifdef _DEBUG
		OutputDebugString(message);
#endif
	}

	static void WriteLine(wstring message)
	{
#ifdef _DEBUG
		message.append(L"\n");
		LPCWSTR str = message.c_str();
		OutputDebugString(str);
#endif
	}

	static void WriteLine(float message)
	{
#ifdef _DEBUG
		auto mes = to_wstring(message);
		mes.append(L"\n");
		LPCWSTR str = mes.c_str();
		OutputDebugString(str);
#endif
	}

	static void WriteLine(wstring m1, float m2)
	{
#ifdef _DEBUG
		auto mes = to_wstring(m2);
		m1.append(mes);
		m1.append(L"\n");
		LPCWSTR str = m1.c_str();
		OutputDebugString(str);
#endif
	}
	static void WriteCurrentDir()
	{
#ifdef _DEBUG
		wchar_t wtext[MAX_PATH];
		LPWSTR result = wtext;
		GetCurrentDirectory(MAX_PATH, result);
		OutputDebugString(result);
#endif
	}
}