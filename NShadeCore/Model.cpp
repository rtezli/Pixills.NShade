#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	m_pDeviceResources = resources;
	m_pMaterial = shared_ptr<Material>();
	m_Vertices = shared_ptr<vector<nshade::Vertex>>(new vector<nshade::Vertex>());
	m_Indices = shared_ptr<vector<unsigned int>>(new vector<unsigned int>());
}

Model::~Model()
{

}

HRESULT Model::Initialize()
{
	return 0;
}

HRESULT Model::LoadModelFromFBXFile(char* fileName)
{
	return nshade::FbxReader::Read(fileName, GetVertices(), GetIndices());
}

HRESULT Model::LoadModelFromOBJFile(char* fileName, bool isRightHand)
{
	return ObjParser::Parse(GetVertices(), GetIndices(), fileName);
}

//HRESULT Model::FillVertexAndIndexBuffer(vector<unsigned int>* modelIndexes, vector<nshade::Vertex>* modelVertices)
//{
//	auto spot = XMFLOAT4{ 5.0f, 5.0f, 0.0f, 1.0f };
//	auto ambient = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 0.0f };
//	auto color = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
//
//	auto input = new vector<PhongShader::InputLayout>();
//
//	for (unsigned int i = 0; i < modelVertices->size(); i++)
//	{
//		auto vertex = modelVertices->at(i);
//
//		auto vertexInput = new PhongShader::InputLayout();
//		vertexInput->Position = vertex.Position;
//		vertexInput->Color = color;
//		vertexInput->Normal = vertex.Normal;
//		vertexInput->AmbientColorIntensity = ambient;
//		vertexInput->LightPositionIntensity = spot;
//		input->push_back(*vertexInput);
//	}
//}

HRESULT Model::CreateVertexBuffer()
{
	//auto material = GetMaterial();
	//auto shader = material->GetShaders()->VertexShader;
	//auto layout = shader->GetLayout();

	//PhongShader::InputLayout* vertexArr;
	//vertexArr = (PhongShader::InputLayout*)malloc(GetVertices()->size() * sizeof(PhongShader::InputLayout));
	//copy(input->begin(), input->end(), vertexArr);

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	//vertexBufferDesc.ByteWidth = sizeof(PhongShader::InputLayout) * m_pDeviceResources->VertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	// vertexBufferData.pSysMem = vertexArr;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	auto result = DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_VertexBuffer);
	if (FAILED(result))
	{
		return result;
	}
}

HRESULT Model::CreateIndexBuffer()
{
	//unsigned int* indexArr;
	//indexArr = (unsigned int*)malloc(GetIndices()->size() * sizeof(unsigned int));
	//copy(modelIndexes->begin(), modelIndexes->end(), indexArr);

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * DeviceResource()->IndexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indexArr;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	return DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_IndexBuffer);
}


HRESULT Model::SetTopology(char verticesPerFace)
{
	switch (verticesPerFace)
	{
		case 3 :
			m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case 4:
			m_Topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	}
	return 0;
}

HRESULT Model::AssignMaterial(Material* pMaterial)
{
	m_pMaterial = shared_ptr<Material>(pMaterial);
	return 0;
}

HRESULT Model::Render()
{
	return 0;
}