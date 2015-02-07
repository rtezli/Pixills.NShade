#include "stdafx.h"
#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

HRESULT Model::Initialize(ID3D11Device* pDevice, std::vector<VertexPositionColor>* pModel, unsigned int size)
{
	m_pDevice = std::shared_ptr<ID3D11Device>(pDevice);
	auto resutlt = InitializeVertexBuffer(pModel);
	resutlt = InitializeIndexBuffer(NULL);
	return 0;
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
			DirectX::XMVECTOR newVector = { (float)data[0], (float)data[1], (float)data[2], (float)data[3] };
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

HRESULT Model::InitializeVertexBuffer(std::vector<VertexPositionColor>* vertices)
{
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = &vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	auto vertBuffer = m_pVertexBuffer.get();
	
	auto result = m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Model::InitializeIndexBuffer(std::vector<int>* indeces)
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

const std::vector<VertexPositionColor> Model::Cube =
{
	VertexPositionColor{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	VertexPositionColor{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	VertexPositionColor{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	VertexPositionColor{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

	VertexPositionColor{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	VertexPositionColor{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	VertexPositionColor{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
	VertexPositionColor{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
};