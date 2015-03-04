#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	m_pDeviceResources = resources;
	m_pMaterial = shared_ptr<Material>();
	m_pVertices = shared_ptr<vector<nshade::Vertex>>(new vector<nshade::Vertex>());
	m_pIndices = shared_ptr<vector<unsigned int>>(new vector<unsigned int>());
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

HRESULT Model::AssignMaterial(Material* pMaterial)
{	
	m_pMaterial = shared_ptr<Material>(pMaterial);
	return 0;
}

HRESULT Model::Render()
{
	return 0;
}