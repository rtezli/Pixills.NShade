#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	_deviceResources = shared_ptr<DeviceResources>(resources);
}

HRESULT Model::Initialize()
{
	_indices = new vector<UINT>();
	_vertices = new vector<NVertex>();

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

	result = FillVertexAndIndexBuffer(_indices, _vertices);
	if (FAILED(result))
	{
		return result;
	}

	return InitializeConstantBuffer();
}

HRESULT Model::LoadModelFromFBXFile(CHAR* fileName)
{
	return nshade::FbxReader::Read(fileName, _vertices, _indices);
}

HRESULT Model::FillVertexAndIndexBuffer(vector<UINT>* modelIndexes, vector<NVertex>* modelVertices)
{
	auto spot = XMFLOAT4{ 5.0f, 5.0f, 0.0f, 1.0f };
	auto ambient = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 0.0f };
	auto color = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	auto input = new  vector<PhongShader::InputLayout>();

	for (UINT i = 0; i < modelVertices->size(); i++)
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

	UINT* indexArr;
	indexArr = (UINT*)malloc(DeviceResource()->IndexCount * sizeof(UINT));
	copy(modelIndexes->begin(), modelIndexes->end(), indexArr);

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(UINT) * DeviceResource()->IndexCount;
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

HRESULT Model::LoadModelFromOBJFile(CHAR* fileName, BOOL isRightHand)
{
	return ObjParser::Parse(_vertices, _indices, fileName);
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
	static const NVertex cube[] =
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
	vertexBufferDesc.ByteWidth = sizeof(NVertex) * ARRAYSIZE(cube);
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

HRESULT Model::InitializeIndexBuffer(INT indeces[])
{
	// 6 faces * 4 vertices = 24 vertices. 
	static const UINT cubeIndices[] =
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
	indexBufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(cubeIndices);
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

HRESULT Model::CreateHorizontalPlane(FLOAT size, XMFLOAT3* position)
{
	static const NVertex plane[] =
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
		_vertices->push_back(plane[i]);
	}

	static const UINT cubeIndices[] =
	{
		4, 3, 1, 
		3, 2, 1
	};

	UINT highest = 0;
	for (UINT i = 0; i < _indices->size(); i++)
	{
		auto current = _indices->at(i);
		if (current > highest)
		{
			highest = current;
		}
	}

	for (auto i = 0; i < 6; i++)
	{
		_indices->push_back(cubeIndices[i] + highest);
	}

	return 0;
}

HRESULT Model::CreateCube(FLOAT size, XMFLOAT3* position)
{
	auto indices = new vector<UINT>();
	auto vertices = new vector<NVertex>();

	FLOAT halfSize = size / 2;
	for (auto i = 0; i < 2; i++)
	{
		for (auto j = 0; j < 2; j++)
		{
			for (auto k = 0; k < 2; k++)
			{
				XMFLOAT3 pos{ halfSize + i, halfSize, halfSize + j };
				auto vertex = new NVertex{ pos };
				_vertices->push_back(*vertex);
			}
		}
	}

	return 0;
}

HRESULT Model::SetTopology(CHAR verticesPerFace)
{
	switch (verticesPerFace)
	{
		case 3 :
			_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case 4:
			_topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	}
	return 0;
}

// TODO : Add material