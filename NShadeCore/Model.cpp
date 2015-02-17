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
	auto result = LoadModelFromFBXFile("../Models/teapot.fbx");
	if (FAILED(result))
	{
		return result;
	}

	result = InitializeVertexBuffer();
	if (FAILED(result))
	{
		return result;
	}

	result = InitializeIndexBuffer(NULL);
	if (FAILED(result))
	{
		return result;
	}

	return InitializeConstantBuffer();
}

HRESULT Model::LoadModelFromFBXFile(char* fileName)
{
	Debug::WriteCurrentDir();
	auto sdkManager = FbxManager::Create();

	auto fbxIOsettings = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(fbxIOsettings);

	auto fbxImporter = FbxImporter::Create(sdkManager, "");
	auto fbxScene = FbxScene::Create(sdkManager, "");

	auto success = fbxImporter->Initialize(fileName, -1, sdkManager->GetIOSettings());
	if (!success)
	{
		return success;
	}

	success = fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	if (!success)
	{
		return success;
	}

	auto fbxRootNode = fbxScene->GetRootNode();
	auto count = fbxRootNode->GetChildCount();

	auto rootMesh = fbxRootNode->GetMesh();
	auto rootGeometry = fbxRootNode->GetGeometry();

	auto modelVertices = new vector<Vertex>();
	auto modelIndexes = new vector<unsigned short>();

	// The scene maybe
	for (auto i = 0; i < count; i++)
	{
		auto child = fbxRootNode->GetChild(i);
		auto childName = child->GetName();
		auto childCount = child->GetChildCount();

		// For each model in the scene
		for (auto k = 0; k < childCount; k++)
		{
			child = child->GetChild(k);

			Debug::WriteLine("FBX : Loading child", childName);

			auto childMesh = child->GetMesh();
			auto childPolygonCount = childMesh->GetPolygonCount();

			// TODO : Add material

			//For each polygon in the model
			for (auto n = 0; n < childPolygonCount; n++)
			{
				auto polySize = childMesh->GetPolygonSize(n);
				auto vertexIndex = childMesh->GetPolygonVertexIndex(n);

				//For each point in a polygon get :  cooradinates, normals and index
				for (auto p = vertexIndex; p < polySize; p++)
				{
					FbxVector4 normal;
					auto vertexPoint	= childMesh->GetPolygonVertex(n, p);
					auto vertexNormal	= childMesh->GetPolygonVertexNormal(n, p, normal);
					auto point = childMesh->GetControlPointAt(vertexPoint);

					auto newVertex = new Vertex();
					auto position = new XMFLOAT3{ (float)point.mData[0], (float)point.mData[1], (float)point.mData[2] };

					newVertex->Position = *position;
					// TODO : Remove color from vertex and apply real model material
					newVertex->Color	= XMFLOAT3{ 0.9f, 0.7f, 1.0f };
					newVertex->Normal	= XMFLOAT3{ (float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2] };
					newVertex->UV		= XMFLOAT3{ 0.0f, 0.0f, 0.0f };

					modelVertices->push_back(*newVertex);
					modelIndexes->push_back(p);
				}
			}
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * modelVertices->size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = modelVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	auto result = DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	DeviceResource()->IndexCount = modelIndexes->size();

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * DeviceResource()->IndexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = modelIndexes;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	// fbxScene->Destroy();
	// fbxRootNode->Destroy();

	return DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &DeviceResource()->IndexBuffer);
}

FbxScene* Model::ImportFbx(char *fileName)
{
	auto lSdkManager = FbxManager::Create();

	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);

	auto lScene = FbxScene::Create(lSdkManager, "");

	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	lImporter->Initialize(fileName, -1, ios);
	lImporter->Import(lScene);
	lImporter->Destroy();

	return lScene;
}

HRESULT Model::LoadModelFromOBJFile(char* fileName)
{
	// TODO : Get the vertices from the file

	Vertex* vertices[] = { 0 };

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
}

HRESULT Model::InitializeConstantBuffer()
{
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ConstantBufferData), D3D11_BIND_CONSTANT_BUFFER);
	return DeviceResource()->Device->CreateBuffer(&constantBufferDesc, nullptr, &DeviceResource()->ConstBuffer);
}

HRESULT Model::InitializeVertexBuffer()
{
	static const Vertex cube[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(cube);
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
	static const unsigned short cubeIndices[] =
	{
		0, 2, 1, 1, 2, 3, 4, 5, 6, 5, 7, 6,
		0, 2, 1, 1, 2, 3, 4, 5, 6, 5, 7, 6,
		0, 1, 5, 0, 5, 4, 2, 6, 7, 2, 7, 3,
		0, 4, 6, 0, 6, 2, 1, 3, 7, 1, 7, 5
	};

	DeviceResource()->IndexCount = ARRAYSIZE(cubeIndices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(cubeIndices);
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

HRESULT Model::CreateVertexAndIndexBuffer(XMFLOAT3* vertices)
{
	long size = 0;
	int* indexBuffer = new int[size];
	Vertex* vertexBuffer = new Vertex[size];

	for (long i = 0; i < size; i++)
	{
		vertexBuffer[i].Position = vertices[i];
		indexBuffer[i] = i;
	}
	return 0;
}