#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "common.h"
#include "d3dcompiler.h"

#ifndef PS_PROFILE
#define PS_PROFILE {"ps_5_0" }
#endif

#ifndef VS_PROFILE
#define VS_PROFILE {"vs_5_0" }
#endif

#ifndef GS_PROFILE
#define GS_PROFILE { "ps_5_0" }
#endif

#ifndef HS_PROFILE
#define HS_PROFILE { "hs_5_0" }
#endif

#ifndef DS_PROFILE
#define DS_PROFILE { "ds_5_0" }
#endif

enum ShaderVersion : CHAR
{
	V0 = 0,
	V1 = 0,
	V2 = 0,
	V3 = 0,
	V4 = 0,
	V5 = 0
};

enum ShaderType : CHAR
{
	VS = 1,
	PS = 2,
	GS = 3,
	HS = 4,
	CS = 5,
	DS = 6
};

class Shader
{
public:
	Shader(DeviceResources *resources);
   ~Shader(); 
public:
	HRESULT	SetVertexShader(LPCWSTR compiledShaderFile);
	HRESULT	CompileVertexShader(LPCWSTR shaderSource);

	HRESULT SetHullShader(LPCWSTR compiledShaderFile);
	HRESULT CompileHullShader(LPCWSTR shaderSource);

	HRESULT SetDomainShader(LPCWSTR compiledShaderFile);
	HRESULT CompileDomainShader(LPCWSTR shaderSource);

	HRESULT SetGeometryShader(LPCWSTR compiledShaderFile);
	HRESULT CompileGeometryShader(LPCWSTR shaderSource);

	HRESULT SetPixelShader(LPCWSTR compiledShaderFile);
	HRESULT CompilePixelShader(LPCWSTR shaderSource);

	HRESULT Set();
private:
	HRESULT CompileShader(LPCWSTR compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile);
private:
	DeviceResources*					_deviceResources;
	DeviceResources*					const Resources(){		return _deviceResources; }
	ID3D11Device*						const Device(){			return _deviceResources->Device; }
	ID3D11DeviceContext*				const DeviceContext(){	return _deviceResources->DeviceContext; }
	vector<D3D11_INPUT_ELEMENT_DESC>	_inputDescription;
	ShaderType							_type;
	ID3D11PixelShader*					_pixelShader;
	ID3D11VertexShader*					_vertexShader;
	ID3D11HullShader*					_hullShader;
	ID3D11DomainShader*					_domainShader;
	ID3D11GeometryShader*				_geometryShader;
	ID3D11ComputeShader*				_computeShader;
};

