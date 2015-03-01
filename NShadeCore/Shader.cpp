#include "stdafx.h"

#include "shader.h"

Shader::Shader(DeviceResources* pResources)
{
	m_pDeviceResources = pResources;
}

Shader::~Shader()
{

}

HRESULT Shader::Load(LPCWSTR file)
{
	switch (Type)
	{
		case ShaderType::VS:
		{
			SetVertexShader(file);
		}

		case ShaderType::PS:
		{
			SetPixelShader(file);
		}

		case ShaderType::GS:
		{
			SetGeometryShader(file);
		}

		case ShaderType::DS:
		{
			SetDomainShader(file);
		}
		case ShaderType::HS:
		{
			SetHullShader(file);
		}
		case ShaderType::CS:
		{
			SetComputeShader(file);
		}
	}
	return 0;
}

HRESULT Shader::Compile(LPCWSTR file)
{
	auto profile = CreateProfile();
	switch (Type)
	{
		case ShaderType::VS:
		{
			CompileVertexShader(file, profile);
		}

		case ShaderType::PS:
		{
			CompilePixelShader(file, profile);
		}

		case ShaderType::GS:
		{
			CompileGeometryShader(file, profile);
		}

		case ShaderType::DS:
		{
			CompileDomainShader(file, profile);
		}
		case ShaderType::HS:
		{
			CompileHullShader(file, profile);
		}
		case ShaderType::CS:
		{
			CompileComputeShader(file, profile);
		}
	}
	return 0;
}


HRESULT Shader::SetVertexShader(LPCWSTR compiledShaderFile)
{
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

HRESULT Shader::CompileVertexShader(LPCWSTR compiledShaderFile, LPCSTR profile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11VertexShader* vertexShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, profile);
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

HRESULT Shader::CompileHullShader(LPCWSTR compiledShaderFile, LPCSTR profile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11HullShader* hullShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, profile);
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

HRESULT Shader::CompileGeometryShader(LPCWSTR compiledShaderFile, LPCSTR profile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11GeometryShader* geometryShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, profile);
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

HRESULT Shader::CompilePixelShader(LPCWSTR compiledShaderFile, LPCSTR profile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11PixelShader* pixelShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, profile);
	if (FAILED(result))
	{
		return result;
	}
	return Device()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader);
}


HRESULT Shader::SetDomainShader(LPCWSTR compiledShaderFile)
{
	auto psByteCode = File::ReadFileBytes(compiledShaderFile);
	return Device()->CreateDomainShader(psByteCode->FileBytes, psByteCode->Length, nullptr, &Resources()->Shaders->DomainShader);
}

HRESULT Shader::CompileDomainShader(LPCWSTR compiledShaderFile, LPCSTR profile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11DomainShader* domainShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, profile);
	if (FAILED(result))
	{
		return result;
	}
	return Device()->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &domainShader);
}


HRESULT Shader::SetComputeShader(LPCWSTR compiledShaderFile)
{
	auto psByteCode = File::ReadFileBytes(compiledShaderFile);
	return Device()->CreatePixelShader(psByteCode->FileBytes, psByteCode->Length, nullptr, &Resources()->Shaders->PixelShader);
}

HRESULT Shader::CompileComputeShader(LPCWSTR compiledShaderFile, LPCSTR profile)
{
	ID3DBlob* shaderBlob = 0;
	ID3D11PixelShader* pixelShader = 0;

	auto result = CompileShader(compiledShaderFile, shaderBlob, profile);
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

HRESULT Shader::Set()
{
	switch (m_Type)
	{
		case ShaderType::VS:
		{
			DeviceContext()->VSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
			DeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
		}

		case ShaderType::PS:
		{
			DeviceContext()->PSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
			DeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
		}

		case ShaderType::GS:
		{
			DeviceContext()->GSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
			DeviceContext()->GSSetShader(m_GeometryShader, nullptr, 0);
		}

		case ShaderType::DS:
		{
			DeviceContext()->DSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
			DeviceContext()->DSSetShader(m_DomainShader, nullptr, 0);
		}
		case ShaderType::HS:
		{
			DeviceContext()->HSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
			DeviceContext()->HSSetShader(m_HullShader, nullptr, 0);
		}
		case ShaderType::CS:
		{
			DeviceContext()->CSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
			DeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
		}
	}
	return 0;
}

HRESULT Shader::Render()
{
	return 0;
}

LPCSTR Shader::CreateProfile()
{
	return "";
}

