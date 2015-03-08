#include "stdafx.h"
#include "model.h"

VOID Model::LoadModelFromFBXFile(CHAR *fileName)
{
	auto vertices = new vector<NVertex>();
	auto indices = new vector<UINT>();

	auto result = nshade::FbxReader::Read(fileName, vertices, indices);

	_vertices = shared_ptr<vector<NVertex>>(vertices);
	_indices = shared_ptr<vector<UINT>>(indices);
}

VOID Model::LoadModelFromOBJFile(CHAR *fileName, BOOL isRightHand)
{
	auto vertices = new vector<NVertex>();
	auto indices = new vector<UINT>();

	auto result = ObjParser::Parse(GetVertices(), GetIndices(), fileName);

	_vertices = shared_ptr<vector<NVertex>>(vertices);
	_indices = shared_ptr<vector<UINT>>(indices);

	CreateBuffers();
}

VOID Model::AssignMaterial(Material *material)
{
	_material = shared_ptr<Material>(material);
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
	 
	auto vertexShader = matrial->Shaders->VertexShader;

	//vertexShader->AppendExtraData(extraData, extraSite);

	//auto extraData = vertexShader->GetExtraData();
	//auto extraSite = vertexShader->GetExtraDataSize();

	vertexShader->CreateBuffers(vertexBuffer, indexBuffer);
}