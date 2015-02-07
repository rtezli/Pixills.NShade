#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	m_pDeviceResources = shared_ptr<DeviceResources>(resources);
}

Model::~Model()
{

}

HRESULT Model::Initialize(vector<VertexPositionColor>* pModel, unsigned int size)
{
	auto result = InitializeVertexBuffer(pModel);
	if (FAILED(result))
	{
		return result;
	}

	result = InitializeIndexBuffer(NULL);
	if (FAILED(result))
	{
		return result;
	}

	result = InitializeConstantBuffer();
	if (FAILED(result))
	{
		return result;
	}
	return result;
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

	VertexPositionColor* vertices = 0;

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(VertexPositionColor) * 3;
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
	VertexPositionColor* vertices = 0;

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(VertexPositionColor) * 3;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
}

HRESULT Model::InitializeVertexBuffer(vector<VertexPositionColor>* vertices)
{
	vector<VertexPositionColor>* verticesVector;
	auto cube = Model::Cube;
	
	if (!vertices)
	{
		verticesVector = &cube;
	}
	else
	{
		verticesVector = vertices;
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = verticesVector;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);

	auto result = DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Model::InitializeIndexBuffer(vector<int>* indeces)
{
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	static const unsigned short cubeIndices[] =
	{
		0, 2, 1, 1, 2, 3,
		4, 5, 6, 5, 7, 6,
		0, 1, 5, 0, 5, 4,
		2, 6, 7, 2, 7, 3,
		0, 4, 6, 0, 6, 2,
		1, 3, 7, 1, 7, 5,
	};
	indexBufferData.pSysMem = &cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(indeces), D3D11_BIND_INDEX_BUFFER);

	auto device = DeviceResource()->Device;
	auto indexBuffer = DeviceResource()->IndexBuffer;

	auto result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	if (FAILED(result))
	{
		return result;
	}
	return result;
}

HRESULT Model::InitializeConstantBuffer()
{
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	auto result = DeviceResource()->Device->CreateBuffer(&constantBufferDesc, nullptr, &DeviceResource()->ConstantBuffer);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

const vector<VertexPositionColor> Model::Cube =
{
	VertexPositionColor{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	VertexPositionColor{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	VertexPositionColor{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	VertexPositionColor{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

	VertexPositionColor{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	VertexPositionColor{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	VertexPositionColor{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
	VertexPositionColor{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
};