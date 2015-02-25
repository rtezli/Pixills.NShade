#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	m_pDeviceResources = shared_ptr<DeviceResources>(resources);
}

Model::~Model()
{

}

HRESULT Model::Initialize()
{
	m_Indices = new vector<unsigned int>();
	m_Vertices = new vector<nshade::Vertex>();

	auto result = LoadModelFromFBXFile("../Models/teapot.fbx");
	if (FAILED(result))
	{
		return result;
	}

	auto position =  XMFLOAT3(0.0f, 0.0f, 0.0f);
	result = CreateHorizontalPlane(15.00f, &position);
	if (FAILED(result))
	{
		return result;
	}

	result = FillVertexAndIndexBuffer(m_Indices, m_Vertices);
	if (FAILED(result))
	{
		return result;
	}

	return InitializeConstantBuffer();
}

HRESULT Model::LoadModelFromFBXFile(char* fileName)
{
	return nshade::FbxReader::Read(fileName, m_Vertices, m_Indices);
}

HRESULT Model::FillVertexAndIndexBuffer(vector<unsigned int>* modelIndexes, vector<nshade::Vertex>* modelVertices)
{
	auto spot = XMFLOAT4{ 5.0f, 5.0f, 0.0f, 1.0f };
	auto ambient = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 0.0f };
	auto color = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	auto input = new  vector<PhongShader::InputLayout>();

	for (unsigned int i = 0; i < modelVertices->size(); i++)
	{
		auto vertex = modelVertices->at(i);

		auto vertexInput = new PhongShader::InputLayout();
		vertexInput->Position = vertex.Position;
		vertexInput->Color = color;
		vertexInput->Normal = vertex.Normal;
		vertexInput->AmbientColorIntensity = ambient;
		vertexInput->LightPositionIntensity = spot;
		input->push_back(*vertexInput);
	}

	DeviceResource()->VertexCount = modelVertices->size();
	DeviceResource()->IndexCount = modelIndexes->size();

	PhongShader::InputLayout* vertexArr;
	vertexArr = (PhongShader::InputLayout*)malloc(DeviceResource()->VertexCount * sizeof(PhongShader::InputLayout));
	copy(input->begin(), input->end(), vertexArr);

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(PhongShader::InputLayout) * DeviceResource()->VertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertexArr;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	auto result = DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	unsigned int* indexArr;
	indexArr = (unsigned int*)malloc(DeviceResource()->IndexCount * sizeof(unsigned int));
	copy(modelIndexes->begin(), modelIndexes->end(), indexArr);

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

	return DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &DeviceResource()->IndexBuffer);
}

HRESULT Model::LoadModelFromOBJFile(char* fileName, bool isRightHand)
{
	return ObjParser::Parse(m_Vertices, m_Indices, fileName);
}

HRESULT Model::InitializeConstantBuffer()
{
	// Belongs to renderer class
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
	constantBufferData.pSysMem = &DeviceResource()->ConstBuffer;
	constantBufferData.SysMemPitch = 0;
	constantBufferData.SysMemSlicePitch = 0;

	return DeviceResource()->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &DeviceResource()->ConstBuffer);
}

HRESULT Model::InitializeVertexBuffer()
{
	static const nshade::Vertex cube[] =
	{
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 1.0f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 1.0f, 0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 1.0f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 1.0f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-15.0f, 0.0f, -15.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, 0.0f, -15.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, 0.0f, 15.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, 0.0f, 15.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },

	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(nshade::Vertex) * ARRAYSIZE(cube);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cube;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	return DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
}

HRESULT Model::InitializeIndexBuffer(int indeces[])
{
	// 6 faces * 4 vertices = 24 vertices. 
	static const unsigned int cubeIndices[] =
	{
		0, 2, 1, 1, 2, 3, // Face 1 (2 Polygons)
		4, 5, 6, 5, 7, 6, // Face 2 (2 Polygons)
		0, 1, 5, 0, 5, 4, // Face 3 (2 Polygons)
		2, 6, 7, 2, 7, 3, // Face 4 (2 Polygons)
		0, 4, 6, 0, 6, 2, // Face 5 (2 Polygons)
		1, 3, 7, 1, 7, 5, // Face 6 (2 Polygons)

		8, 10, 11, 8, 9, 10 // F7 (The Plane)
	};

	DeviceResource()->IndexCount = ARRAYSIZE(cubeIndices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * ARRAYSIZE(cubeIndices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	auto result = DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &DeviceResource()->IndexBuffer);
	if (FAILED(result))
	{
		return result;
	}
	return result;
}

HRESULT Model::CreateHorizontalPlane(float size, XMFLOAT3* position)
{
	static const nshade::Vertex plane[] =
	{
		{ 
			XMFLOAT3(size * -1.0f + position->x, position->y, size * -1.0f + position->z),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
			XMFLOAT3(0.0f, 1.0f, 0.0f), 
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 0.0f)
		},
		{ 
			XMFLOAT3(size + position->x, position->y, size * -1.0f + position->z),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 0.0f)
		},
		{ 
			XMFLOAT3(size + position->x, position->y, size + position->z),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 0.0f)
		},
		{ 
			XMFLOAT3(size * -1.0f + position->x, position->y, size + position->z),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
			XMFLOAT3(0.0f, 1.0f, 0.0f), 
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 0.0f) 
		}
	};

	for (auto i = 0; i < 4; i++)
	{
		m_Vertices->push_back(plane[i]);
	}

	static const unsigned int cubeIndices[] =
	{
		4, 3, 1, 
		3, 2, 1
	};

	unsigned int highest = 0;
	for (unsigned int i = 0; i < m_Indices->size(); i++)
	{
		auto current = m_Indices->at(i);
		if (current > highest)
		{
			highest = current;
		}
	}

	for (auto i = 0; i < 6; i++)
	{
		m_Indices->push_back(cubeIndices[i] + highest);
	}

	return 0;
}

HRESULT Model::CreateCube(float size, XMFLOAT3* position)
{
	auto indices = new vector<unsigned int>();
	auto vertices = new vector<nshade::Vertex>();

	float halfSize = size / 2;
	for (auto i = 0; i < 2; i++)
	{
		for (auto j = 0; j < 2; j++)
		{
			for (auto k = 0; k < 2; k++)
			{
				XMFLOAT3 pos{ halfSize + i, halfSize, halfSize + j };
				auto vertex = new nshade::Vertex{ pos };
				m_Vertices->push_back(*vertex);
			}
		}
	}

	return 0;
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

// TODO : Add material