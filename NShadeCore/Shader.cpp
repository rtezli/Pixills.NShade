#include "stdafx.h"

#include "shader.h"

Shader::Shader(DeviceResources* pResources)
{
	m_pDeviceResources = pResources;
}

Shader::~Shader()
{
}


HRESULT Shader::SetVertexShader(LPCWSTR compiledShaderFile)
{
	//ID3D11ClassLinkage linkage;
	Debug::WriteCurrentDir();
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = Device()->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, nullptr, &Resources()->Shaders->VertexShader);

	if (FAILED(result))
	{
		return result;
	}

	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex position
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Vertex color
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Normal vector
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Texture UV
		{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Ambient Light color where w is intensity
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }  // Point light position w is intensity
	};

	return Device()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->FileBytes, vsByteCode->Length, &Resources()->InputLayout);
}

HRESULT Shader::CompileVertexShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11VertexShader* vertexShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, VS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return Device()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader);
}

HRESULT Shader::SetHullShader(LPCWSTR compiledShaderFile)
{
	auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = Resources()->Shaders;
	return Device()->CreateHullShader(hsByteCode->FileBytes, hsByteCode->Length, nullptr, &shaders->HullShader);
}

HRESULT Shader::CompileHullShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11HullShader* hullShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, HS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return Device()->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &hullShader);
}

HRESULT Shader::SetGeometryShader(LPCWSTR compiledShaderFile)
{
	auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = Resources()->Shaders;
	return Device()->CreateGeometryShader(gsByteCode->FileBytes, gsByteCode->Length, nullptr, &shaders->GeometryShader);
}

HRESULT Shader::CompileGeometryShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11GeometryShader* geometryShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, GS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return Device()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &geometryShader);
}

HRESULT Shader::SetPixelShader(LPCWSTR compiledShaderFile)
{
	auto psByteCode = File::ReadFileBytes(compiledShaderFile);
	return Device()->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, nullptr, &Resources()->Shaders->PixelShader);
}

HRESULT Shader::CompilePixelShader(LPCWSTR compiledShaderFile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11PixelShader* pixelShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, PS_PROFILE);
	if (FAILED(result))
	{
		return result;
	}
	return Device()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader);
}

HRESULT Shader::CompileShader(LPCWSTR compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile)
{
	ID3DBlob* shaderBlob = 0;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE",
		"1",
		nullptr,
		nullptr
	};

	return D3DCompileFromFile(compiledShaderFile,
		nullptr, // Defines
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", // Entrypoint
		shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}