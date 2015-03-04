#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	m_pDeviceResources = resources;
}

Model::~Model()
{

}

void Model::LoadModelFromFBXFile(char* fileName)
{
	auto vertices = new vector<nshade::Vertex>();
	auto indices = new vector<unsigned int>();

	auto result = nshade::FbxReader::Read(fileName, vertices, indices);

	m_pVertices = shared_ptr<vector<nshade::Vertex>>(vertices);
	m_pIndices = shared_ptr<vector<unsigned int>>(indices);
	
	CreateBuffers();
}

void Model::LoadModelFromOBJFile(char* fileName, bool isRightHand)
{
	auto vertices = new vector<nshade::Vertex>();
	auto indices = new vector<unsigned int>();

	auto result = ObjParser::Parse(GetVertices(), GetIndices(), fileName);
	
	m_pVertices = shared_ptr<vector<nshade::Vertex>>(vertices);
	m_pIndices = shared_ptr<vector<unsigned int>>(indices);

	CreateBuffers();
}

void Model::AssignMaterial(Material* pMaterial)
{	
	m_pMaterial = shared_ptr<Material>(pMaterial);
}

void Model::CreateBuffers()
{
	/* Vertex Buffer */
	auto vertices = GetVertices();
	
	m_pStrides = sizeof(nshade::Vertex);

	D3D11_BUFFER_DESC  vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = m_pStrides * vertices->size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = &vertices[0];
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	auto result = m_pDeviceResources->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	m_pVertexBuffer = shared_ptr<ID3D11Buffer>(vertexBuffer);


	/* Index Buffer */
	auto indices = GetIndices();

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices->size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;


	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = &indices[0];
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	result = m_pDeviceResources->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	m_pIndexBuffer = shared_ptr<ID3D11Buffer>(indexBuffer);
}