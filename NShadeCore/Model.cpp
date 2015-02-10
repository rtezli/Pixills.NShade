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
	auto result = InitializeVertexBuffer();
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

	FbxScene* fbxScene = ImportFbx(fileName);
	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	FbxGeometry* geometry = 0;
	FbxArray<FbxVector4>* pointArray;

	auto count = fbxRootNode->GetChildCount();
	for (auto i = 0; i < count; i++)
	{
		auto child = fbxRootNode->GetChild(i);
		FbxMesh* pMesh = (FbxMesh*)child->GetNodeAttribute();
		pointArray = &geometry->mControlPoints;

		auto size = pointArray->Size();
		for (auto n = 0; n < size; n++)
		{
			auto point = pointArray->GetAt(i);
			auto data = point.mData;
			XMVECTOR newVector = { (float)data[0], (float)data[1], (float)data[2], (float)data[3] };
		}
	}
	fbxScene->Destroy();

	XMFLOAT3* vertices[] = {0};

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(XMFLOAT3) * ARRAYSIZE(vertices);
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
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

	XMFLOAT3* vertices[] = { 0 };

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(XMFLOAT3) * ARRAYSIZE(vertices);
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
}

HRESULT Model::InitializeConstantBuffer()
{
	Debug::WriteLine(L"CALL : Model::InitializeConstantBuffer\t\t\t(Device->CreateBuffer : Constant Buffer)\n");
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ConstantBufferData), D3D11_BIND_CONSTANT_BUFFER);
	return DeviceResource()->Device->CreateBuffer(&constantBufferDesc, nullptr, &DeviceResource()->ConstBuffer);
}

HRESULT Model::InitializeVertexBuffer()
{
	Debug::WriteLine(L"CALL : Model::InitializeVertexBuffer\n");
	static const XMFLOAT3 cube[] =
	{
		// BOTTOM FACE
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f)},
		{ XMFLOAT3( 0.5f, -0.5f, -0.5f)},
		{ XMFLOAT3( 0.5f, -0.5f,  0.5f)},
		{ XMFLOAT3(-0.5f, -0.5f,  0.5f)},

		// TOP FACE
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f)},
		{ XMFLOAT3(-0.5f, 0.5f,  0.5f)},
		{ XMFLOAT3( 0.5f, 0.5f, -0.5f)},
		{ XMFLOAT3( 0.5f, 0.5f,  0.5f)}


	};
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * ARRAYSIZE(cube);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cube;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	Debug::WriteLine(L"CALL : Model::InitializeConstantBuffer\t\t\t(Device->CreateBuffer : Vertex Buffer)\n");
	return DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
}

HRESULT Model::InitializeIndexBuffer(int indeces[])
{
	Debug::WriteLine(L"CALL : Model::InitializeIndexBuffer\n");
	static const unsigned short cubeIndices[] =
	{ 
		0, 1, 2
		//0, 2, 1, 1, 2, 3, 4, 5, 6, 5, 7, 6
		//0, 2, 1, 1, 2, 3, 4, 5, 6, 5, 7, 6,
		//0, 1, 5, 0, 5, 4, 2, 6, 7, 2, 7, 3,
		//0, 4, 6, 0, 6, 2, 1, 3, 7, 1, 7, 5,
	};

	DeviceResource()->IndexCount = ARRAYSIZE(cubeIndices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * ARRAYSIZE(cubeIndices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	Debug::WriteLine(L"CALL : Model::InitializeConstantBuffer\t\t\t(Device->CreateBuffer : Index Buffer)\n");
	auto result = DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &DeviceResource()->IndexBuffer);
	if (FAILED(result))
	{
		return result;
	}
	return result;
}