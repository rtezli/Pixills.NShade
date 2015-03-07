#include "stdafx.h"
#include "vertexshader.h"

VertexShader::VertexShader(DeviceResources* pResources) //: Shader(pResources)
{
	PResources = pResources;	
}

VOID VertexShader::Load(CHAR *fileName)
{
	PByteCode = File::ReadFileBytes(fileName);
	auto result = PResources->Device->CreateVertexShader(PByteCode->Bytes, PByteCode->Length, NULL, &PVertexShader);
}

VOID VertexShader::Compile(char* file, ShaderVersion version)
{

}

VOID VertexShader::SetExtraDataSize(UINT size)
{
	ExtraInputDataSize = size;
	InputDataSize = sizeof(NVertex) + size;
}

VOID VertexShader::CreateBuffers(vector<NVertex> *vertices, vector<UINT> *indices)
{
	// TODO : Merge the vertices with the InputLayout !

	D3D11_BUFFER_DESC  vertexBufferDesc;
	vertexBufferDesc.ByteWidth = sizeof(NVertex) * vertices->size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	vertexBufferData.pSysMem = &vertices[0];
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	auto result = PResources->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	PVertexBuffer = shared_ptr<ID3D11Buffer>(vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(CHAR) * indices->size();
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = &indices[0];
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	indexBufferDesc.StructureByteStride = 0;

	ID3D11Buffer* indexBuffer;
	result = PResources->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	PIndexBuffer = shared_ptr<ID3D11Buffer>(indexBuffer);
}

