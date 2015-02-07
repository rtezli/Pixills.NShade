#pragma once

#include "d3d11.h"
#include "dxgi.h"
//#include "directxmath.h"

struct VertexPositionColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

struct MVPConstantBuffer
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