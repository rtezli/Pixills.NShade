#pragma once

#include "d3d11.h"

enum Perspective : char
{
	UNDEFINED = 0,
	FIRST_PERSON = 1,
	THIRD_PERSON = 2,
	ISOMETRIC = 3
};

struct VertexPositionColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
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
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
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
	static const DirectX::XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 90-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation90(
		0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 180-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation180(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 270-degree Z-rotation
	static const DirectX::XMFLOAT4X4 Rotation270(
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
#ifdef _PRINT_DEBUG
		OutputDebugString(message);
#endif
	}

	static void WriteLine(std::wstring message)
	{
		message.append(L"\n");
		LPCWSTR str = message.c_str();
#ifdef _PRINT_DEBUG
		OutputDebugString(str);
#endif
	}

	static void WriteLine(float message)
	{
		auto mes = std::to_wstring(message);
		mes.append(L"\n");
		LPCWSTR str = mes.c_str();
#ifdef _PRINT_DEBUG
		OutputDebugString(str);
#endif
	}

	static void WriteLine(std::wstring m1, float m2)
	{
		auto mes = std::to_wstring(m2);
		m1.append(mes);
		m1.append(L"\n");
		LPCWSTR str = m1.c_str();
#ifdef _PRINT_DEBUG
		OutputDebugString(str);
#endif
	}
}