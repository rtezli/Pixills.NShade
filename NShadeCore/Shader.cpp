#include "stdafx.h"

#include "shader.h"

Shader::Shader(DeviceResources *resources)
{
	_resources = resources;
}

HRESULT Shader::SetVertexShader(LPCWSTR compiledShaderFile)
{
	//ID3D11ClassLinkage linkage;
	Debug::WriteCurrentDir();
	auto vsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto result = _resources->Device->CreateVertexShader(vsByteCode->FileBytes, vsByteCode->Length, NULL, &_resources->Shaders->VertexShader);

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

	return  _resources->Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsByteCode->FileBytes, vsByteCode->Length, &_resources->InputLayout);
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
	return _resources->Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

HRESULT Shader::SetHullShader(LPCWSTR compiledShaderFile)
{
	auto hsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = _resources->Shaders;
	return _resources->Device->CreateHullShader(hsByteCode->FileBytes, hsByteCode->Length, NULL, &shaders->HullShader);
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
	return _resources->Device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &hullShader);
}

HRESULT Shader::SetGeometryShader(LPCWSTR compiledShaderFile)
{
	auto gsByteCode = File::ReadFileBytes(compiledShaderFile);
	auto shaders = _resources->Shaders;
	return _resources->Device->CreateGeometryShader(gsByteCode->FileBytes, gsByteCode->Length, NULL, &shaders->GeometryShader);
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
	return _resources->Device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &geometryShader);
}

HRESULT Shader::SetPixelShader(LPCWSTR compiledShaderFile)
{
	auto psByteCode = File::ReadFileBytes(compiledShaderFile);
	return _resources->Device->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, NULL, &_resources->Shaders->PixelShader);
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
	return _resources->Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
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
		NULL,
		NULL
	};

	return D3DCompileFromFile(compiledShaderFile,
		NULL, // Defines
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", // Entrypoint
		shaderProfile, flags, 0, &shaderBlob, &shaderBlob);
}

HRESULT Shader::Set()
{
	switch (_type)
	{
		case ShaderType::VS:
		{
			_resources->DeviceContext->VSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
			_resources->DeviceContext->VSSetShader(_vertexShader, NULL, 0);
		}

		case ShaderType::PS:
		{
			_resources->DeviceContext->PSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
			_resources->DeviceContext->PSSetShader(_pixelShader, NULL, 0);
		}

		case ShaderType::GS:
		{
			_resources->DeviceContext->GSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
			_resources->DeviceContext->GSSetShader(_geometryShader, NULL, 0);
		}

		case ShaderType::DS:
		{
			_resources->DeviceContext->DSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
			_resources->DeviceContext->DSSetShader(_domainShader, NULL, 0);
		}
		case ShaderType::HS:
		{
			_resources->DeviceContext->HSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
			_resources->DeviceContext->HSSetShader(_hullShader, NULL, 0);
		}
		case ShaderType::CS:
		{
			_resources->DeviceContext->CSSetConstantBuffers(0, 1, &_resources->ConstBuffer);
			_resources->DeviceContext->CSSetShader(_computeShader, NULL, 0);
		}
	}
	return 0;
}