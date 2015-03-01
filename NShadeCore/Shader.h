#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "includes.h"
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

enum ShaderVersion : char
{
	V0 = 0,
	V1 = 0,
	V2 = 0,
	V3 = 0,
	V4 = 0,
	V5 = 0
};

enum ShaderType : char
{
	VS = 1,
	PS = 2,
	GS = 3,
	HS = 4,
	CS = 5,
	DS = 6
};

struct SHADER_DESC
{
	ShaderType Type;
	ShaderVersion Version;
	LPCSTR FilePath;
};

class Shader
{
public:
	Shader(DeviceResources* pResources);
   ~Shader(); 
protected:
	DeviceResources*					const Resources(){ return m_pDeviceResources; }
	ID3D11Device*						const Device(){ return m_pDeviceResources->Device; }
	ID3D11DeviceContext*				const DeviceContext(){ return m_pDeviceResources->DeviceContext; }
	ShaderType							Type;
	ShaderVersion						Version;
	LPCWSTR								ShaderFilePath;

	HRESULT Initialize();
	HRESULT Load(LPCWSTR file);
	HRESULT Compile(LPCWSTR file);
	HRESULT Set();
	HRESULT Render();

	//virtual struct InputLayout			InputLayout;
	//virtual struct ConstantBuffer		ConstantBuffer;
	//virtual D3D11_INPUT_ELEMENT_DESC*	InputDescription;

private:
	HRESULT	SetVertexShader(LPCWSTR compiledShaderFile);
	HRESULT	CompileVertexShader(LPCWSTR shaderSource, LPCSTR profile);

	HRESULT SetHullShader(LPCWSTR compiledShaderFile);
	HRESULT CompileHullShader(LPCWSTR shaderSource, LPCSTR profile);

	HRESULT SetDomainShader(LPCWSTR compiledShaderFile);
	HRESULT CompileDomainShader(LPCWSTR shaderSource, LPCSTR profile);

	HRESULT SetGeometryShader(LPCWSTR compiledShaderFile);
	HRESULT CompileGeometryShader(LPCWSTR shaderSource, LPCSTR profile);

	HRESULT SetPixelShader(LPCWSTR compiledShaderFile);
	HRESULT CompilePixelShader(LPCWSTR shaderSource, LPCSTR profile);

	HRESULT SetComputeShader(LPCWSTR compiledShaderFile);
	HRESULT CompileComputeShader(LPCWSTR shaderSource, LPCSTR profile);

	LPCSTR Shader::CreateProfile();

	HRESULT CompileShader(LPCWSTR compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile);

	DeviceResources*					m_pDeviceResources;
	vector<D3D11_INPUT_ELEMENT_DESC>	m_inputDescription;
	ShaderType							m_Type;
	ID3D11PixelShader*					m_PixelShader;
	ID3D11VertexShader*					m_VertexShader;
	ID3D11HullShader*					m_HullShader;
	ID3D11DomainShader*					m_DomainShader;
	ID3D11GeometryShader*				m_GeometryShader;
	ID3D11ComputeShader*				m_ComputeShader;
};

