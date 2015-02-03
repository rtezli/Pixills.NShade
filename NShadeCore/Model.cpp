#include "stdafx.h"
#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

HRESULT Model::Initialize(ID3D11Device* pDevice, std::vector<NSVERTEX2>* pModel, UINT size)
{
	m_pDevice = std::shared_ptr<ID3D11Device>(pDevice);
	auto resutlt = InitializeVertexBuffer(pModel);
	resutlt = InitializeIndexBuffer(pModel);
	return 0;
}

HRESULT Model::LoadModelFromFBXFile(CHAR* fileName)
{
	auto fbxScene = FbxImport(fileName);
	FillVerticesFromFbxImport(fbxScene);
	NSVERTEX2* vertices = 0;

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(NSVERTEX2) * 3;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
}

FbxScene* Model::FbxImport(CHAR* fileName)
{
	FbxManager *lSdkManager = FbxManager::Create();
	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	// set some IOSettings options 
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	// create an empty scene
	auto lScene = FbxScene::Create(lSdkManager, "");
	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	// Initialize the importer by providing a filename and the IOSettings to use
	lImporter->Initialize(fileName, -1, ios);
	// Import the scene.
	lImporter->Import(lScene);
	// Destroy the importer.
	lImporter->Destroy();

	return lScene;
}

void Model::FillVerticesFromFbxImport(FbxScene* scene)
{
	FbxGeometry* geometry;
	FbxArray<FbxVector4>* pointArray;
	auto count = scene->GetGeometryCount();
	for (auto i = 0; i < count; i++)
	{
		geometry = scene->GetGeometry(i);
		pointArray = &geometry->mControlPoints;
		auto size = pointArray->Size();
		for (auto n = 0; n < size; n++)
		{
			auto point = pointArray->GetAt(i);
			auto data = point.mData;
			DirectX::XMVECTOR newVector = { (float)data[0], (float)data[1], (float)data[2], (float)data[3] };
		}
	}
	scene->Destroy();
}

HRESULT Model::LoadModelFromOBJFile(CHAR* fileName)
{
	// TODO : Get the vertices from the file
	NSVERTEX2* vertices = 0;

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(NSVERTEX2) * 3;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
}

HRESULT Model::InitializeVertexBuffer(std::vector<NSVERTEX2>* vertices)
{
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = &vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	auto vertBuffer = m_pVertexBuffer.get();
	auto result = m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);
	return result;
}

HRESULT Model::InitializeIndexBuffer(std::vector<NSVERTEX2>* indeces)
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
	auto indexBuffer = m_pIndexBuffer.get();
	auto result = m_pDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	return result;
}

const std::vector<Model::NSVERTEX2> Model::Cube =
{
	NSVERTEX2{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	NSVERTEX2{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	NSVERTEX2{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	NSVERTEX2{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

	NSVERTEX2{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	NSVERTEX2{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	NSVERTEX2{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
	NSVERTEX2{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
};