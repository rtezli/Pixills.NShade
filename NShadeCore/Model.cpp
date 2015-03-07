#include "stdafx.h"
#include "model.h"

VOID Model::LoadModelFromFBXFile(CHAR *fileName)
{
	auto vertices = new vector<NVertex>();
	auto indices = new vector<UINT>();

	auto result = nshade::FbxReader::Read(fileName, vertices, indices);

	m_pVertices = shared_ptr<vector<NVertex>>(vertices);
	m_pIndices = shared_ptr<vector<UINT>>(indices);
}

VOID Model::LoadModelFromOBJFile(CHAR *fileName, BOOL isRightHand)
{
	auto vertices = new vector<NVertex>();
	auto indices = new vector<UINT>();

	auto result = ObjParser::Parse(GetVertices(), GetIndices(), fileName);

	m_pVertices = shared_ptr<vector<NVertex>>(vertices);
	m_pIndices = shared_ptr<vector<UINT>>(indices);

	CreateBuffers();
}

VOID Model::AssignMaterial(Material *pMaterial)
{
	m_pMaterial = shared_ptr<Material>(pMaterial);
}

VOID Model::CreateBuffers()
{
	// Check if Material		== null
	// Check if Shaders			== null
	// Check if VertexShader	== null
	// If null, assign standard shader
	auto vertexBuffer = GetVertices();
	auto indexBuffer = GetIndices();
	auto matrial = GetMaterial();
	 
	auto shaders = matrial->Shaders;
	auto vertexShader = shaders->VertexShader;

	CHAR* extraData = { 0 };
	UINT size = 0;
	vertexShader->CreateExtraData(extraData, size);
	vertexShader->CreateBuffers(vertexBuffer, indexBuffer);
}