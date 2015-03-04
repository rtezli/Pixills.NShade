#pragma once

#include "common.h"
#include "pixelshader.h"
#include "vertexshader.h"

//struct ShaderSet
//{
//	ID3D11PixelShader*		PixelShader;
//	ID3D11VertexShader*		VertexShader;
//	ID3D11HullShader*		HullShader;
//	ID3D11DomainShader*		DomainShader;
//	ID3D11GeometryShader*	GeometryShader;
//	ID3D11ComputeShader*	ComputeShader;
//};

struct ShaderSet
{
	PixelShader*			PixelShader;
	VertexShader*			VertexShader;

	ID3D11HullShader*		HullShader;
	ID3D11DomainShader*		DomainShader;
	ID3D11GeometryShader*	GeometryShader;
	ID3D11ComputeShader*	ComputeShader;
};

class Material
{
public:
	Material();
	~Material();
public:
	ID3D11Texture2D*	const Getexture(){ return Texture.get(); }
	ID3D11Texture2D*	const GetBumpMap(){ return BumpMap.get(); }
	ID3D11Texture2D*	const GetDisplacementMap(){ return DisplacementMap.get(); }
	ShaderSet*			const GetShaders(){ return Shaders.get(); }
public:
	XMFLOAT4					Color;
	shared_ptr<ID3D11Texture2D>	Texture;
	shared_ptr<ID3D11Texture2D>	BumpMap;
	shared_ptr<ID3D11Texture2D>	DisplacementMap;
	shared_ptr<ShaderSet>		Shaders;
	float						SpecularPower;
};

