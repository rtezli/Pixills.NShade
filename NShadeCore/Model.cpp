#include "stdafx.h"
#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::LoadModelFromFBXFile(char* fileName)
{
	auto vertices = new vector<nshade::Vertex>();
	auto indices = new vector<unsigned int>();

	auto result = nshade::FbxReader::Read(fileName, vertices, indices);

	m_pVertices = shared_ptr<vector<nshade::Vertex>>(vertices);
	m_pIndices = shared_ptr<vector<unsigned int>>(indices);
	
	CreateBuffers();
}

void Model::LoadModelFromOBJFile(char* fileName, bool isRightHand)
{
	auto vertices = new vector<nshade::Vertex>();
	auto indices = new vector<unsigned int>();

	auto result = ObjParser::Parse(GetVertices(), GetIndices(), fileName);
	
	m_pVertices = shared_ptr<vector<nshade::Vertex>>(vertices);
	m_pIndices = shared_ptr<vector<unsigned int>>(indices);

	CreateBuffers();
}

void Model::AssignMaterial(Material* pMaterial)
{	
	m_pMaterial = shared_ptr<Material>(pMaterial);
}

void Model::CreateBuffers()
{
	// Check if Material		== null
	// Check if Shaders			== null
	// Check if VertexShader	== null
	// If null, assign standard shader
	GetMaterial()->Shaders->VertexShader->SetBuffers(GetVertices(), GetIndices());
}