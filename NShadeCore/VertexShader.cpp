#include "stdafx.h"
#include "vertexshader.h"

VertexShader::VertexShader(DeviceResources * deviceResources)
{
	_Resources = deviceResources;
}

VOID VertexShader::Load(CHAR *fileName)
{
	_ByteCode = File::ReadFileBytes(fileName);
	auto result = _Resources->Device->CreateVertexShader(_ByteCode->Bytes, _ByteCode->Length, NULL, &_VertexShader);
}

VOID VertexShader::Compile(CHAR *file, ShaderVersion version)
{

}

VOID VertexShader::CreateBuffers(vector<NVertex> *vertices, vector<UINT> *indices)
{
	/* Vertex Buffer */

	NVertex* vertexArr;
	UINT verticesSize = vertices->size();
	vertexArr = (NVertex*)malloc(verticesSize * sizeof(NVertex));
	copy(vertices->begin(), vertices->end(), vertexArr);

	D3D11_BUFFER_DESC  vertexBufferDesc;
	vertexBufferDesc.ByteWidth = sizeof(NVertex) * verticesSize;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	vertexBufferData.pSysMem = vertexArr;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	auto result = _Resources->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	_VertexBuffer = shared_ptr<ID3D11Buffer>(vertexBuffer);

	/* Index Buffer */

	UINT* indexArr;
	UINT indicesSize = indices->size();
	indexArr = (unsigned int*)malloc(indicesSize * sizeof(UINT));
	copy(indices->begin(), indices->end(), indexArr);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indicesSize;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indexArr;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	indexBufferDesc.StructureByteStride = 0;

	ID3D11Buffer* indexBuffer;
	result = _Resources->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	_IndexBuffer = shared_ptr<ID3D11Buffer>(indexBuffer);
}