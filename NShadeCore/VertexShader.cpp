#include "stdafx.h"
#include "vertexshader.h"

VertexShader::VertexShader(DeviceResources* pResources) //: Shader(pResources)
{
	PResources = pResources;	
}

VertexShader::~VertexShader()
{

}

void VertexShader::Load(char* fileName)
{
	PByteCode = File::ReadFileBytes(fileName);
	auto result = PResources->Device->CreateVertexShader(PByteCode->Bytes, PByteCode->Length, nullptr, &PVertexShader);
}

void VertexShader::Compile(char* file, ShaderVersion version)
{

}

void VertexShader::SetBuffers(vector<nshade::Vertex>* vertices, vector<unsigned int>* indices)
{
	// TODO : Merge the vertices with the InputLayout !

	nshade::Vertex* vertexArr;
	vertexArr = (nshade::Vertex*)malloc(vertices->size() * sizeof(nshade::Vertex));
	copy(vertices->begin(), vertices->end(), vertexArr);

	D3D11_BUFFER_DESC  vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = ByteWidth * vertices->size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertexArr;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	auto result = PResources->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	PVertexBuffer = shared_ptr<ID3D11Buffer>(vertexBuffer);
	delete vertexArr;

	/* Index Buffer */

	unsigned int* indexArr;
	indexArr = (unsigned int*)malloc(indices->size() * sizeof(unsigned int));
	copy(indices->begin(), indices->end(), indexArr);

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices->size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indexArr;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	result = PResources->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	PIndexBuffer = shared_ptr<ID3D11Buffer>(indexBuffer);
	delete indexArr;
}