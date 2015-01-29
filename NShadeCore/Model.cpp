#include "stdafx.h"
#include "Model.h"

NSHADECORE_API struct PositionColorVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Col;
}PositionColor;

NShade::Model::Model()
{

}

NShade::Model::~Model()
{

}

HRESULT NShade::Model::LoadModelFromFBXFile(CHAR* fileName)
{
	auto fbxScene = FbxImport(fileName);
	FillVerticesFromFbxImport(fbxScene);
	PositionColorVertex* vertices = 0;

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(PositionColorVertex) * 3;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	CreateBuffer();

	return 0;
}

FbxScene* FbxImport(CHAR* fileName)
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

void FillVerticesFromFbxImport(FbxScene* scene)
{
	FbxGeometry* geometry;
	FbxArray<FbxVector4>* pointArray;
	auto count = scene->GetGeometryCount();
	for (auto i = 0; i < count; i++)
	{
		geometry = scene->GetGeometry(i);
		pointArray = &geometry->mControlPoints;
		for (auto n = 0; n < pointArray->Size; n++)
		{
			auto point = pointArray->GetAt(i);
			auto data = point.mData;
			XMVECTOR newVector = { data[0], data[1], data[2], data[3] };
		}
	}
	scene->Destroy();
}

HRESULT NShade::Model::LoadModelFromOBJFile(CHAR* fileName)
{
	// TODO : Get the vertices from the file
	PositionColorVertex* vertices = 0;

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(PositionColorVertex) * 3;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	CreateBuffer();

	return 0;
}

HRESULT NShade::Model::CreateBuffer()
{
	auto result = m_pDevice->CreateBuffer(&m_bufferDesc, &m_initData, &m_pVertexBuffer);
	return result;
}