#include "stdafx.h"
#include "vertexshader.h"

VertexShader::VertexShader(DeviceResources* pResources) //: Shader(pResources)
{
	PResources = pResources;
}


VertexShader::~VertexShader()
{

}

void VertexShader::Render(ID3D11Buffer* constBuffer)
{
	//PResources->DeviceContext->VSSetConstantBuffers(0, 1, &constBuffer);
	//PResources->DeviceContext->VSSetShader(PVertexShader, nullptr, 0);
}

void VertexShader::Setup(vector<nshade::Vertex>* vertices, vector<unsigned int>* indices)
{
	//nshade::Vertex* vertexArr;
	//vertexArr = (nshade::Vertex*)malloc(vertices->size() * sizeof(nshade::Vertex));
	//copy(input->begin(), input->end(), vertexArr);

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth =  ByteWidth * vertices->size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = &vertices[0];
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	auto result = PResources->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &PVertexBuffer);

	//delete vertexArr;

	//unsigned int* indexArr;
	//indexArr = (unsigned int*)malloc(indices->size() * sizeof(unsigned int));
	//copy(indices->begin(), indices->end(), indexArr);

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * PResources->IndexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = &indices[0];
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	result = PResources->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &PIndexBuffer);

	//delete indexArr;
}

void VertexShader::Load(char* fileName)
{
	ByteCode = File::ReadFileBytes(fileName);
	auto result = PResources->Device->CreateVertexShader(ByteCode->Bytes, ByteCode->Length, nullptr, &PVertexShader);
}

void VertexShader::Compile(char* file, ShaderVersion version)
{

}