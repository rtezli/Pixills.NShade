#include "stdafx.h"
#include "model.h"

VOID Model::LoadModelFromFBXFile(CHAR *fileName)
{
	auto vertices = new vector<NVertex>();
	auto indices = new vector<UINT>();

	auto result = nshade::FbxReader::Read(fileName, vertices, indices);

	_vertices = shared_ptr<vector<NVertex>>(vertices);
	_vertexCount = _vertices->size();
	_indices = shared_ptr<vector<UINT>>(indices);
	_indexCount = _indices->size();
}

VOID Model::LoadModelFromOBJFile(CHAR *fileName, BOOL isRightHand)
{
	auto vertices = new vector<NVertex>();
	auto indices = new vector<UINT>();

	auto result = ObjParser::Parse(GetVertices(), GetIndices(), fileName);

	_vertices = shared_ptr<vector<NVertex>>(vertices);
	_vertexCount = _vertices->size();
	_indices = shared_ptr<vector<UINT>>(indices);
	_indexCount = _indices->size();

	CreateBuffers();
}

VOID Model::AssignMaterial(Material *material)
{
	_material = shared_ptr<Material>(material);
}

VOID Model::CreateBuffers()
{
	auto vertexBuffer = GetVertices();
	auto indexBuffer = GetIndices();
	auto matrial = GetMaterial();	 
	auto vertexShader = matrial->Shaders->VertexShader;
	vertexShader->CreateBuffers(vertexBuffer, indexBuffer);
}