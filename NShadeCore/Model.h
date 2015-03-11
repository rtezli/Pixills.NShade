#pragma once

#include "common.h"
#include "export.h"
#include "material.h"

EXTERN class API Model
{
public:
	void LoadModelFromFBXFile(CHAR *fileName);
	void LoadModelFromOBJFile(CHAR *fileName, BOOL isRightHand);
	void AssignMaterial(Material *material);

	Material*			const GetMaterial(){ return _material.get(); }

	vector<UINT>*		const GetIndices(){ return _indices.get(); }
	UINT				const GetIndexCount(){ return _indexCount; }

	vector<NVertex>*	const GetVertices(){ return _vertices.get(); }
	UINT				const GetVertexCount(){ return _vertexCount; }
	void				CreateBuffers();
private:	
	shared_ptr<Material>		_material;
	shared_ptr<vector<NVertex>>	_vertices;
	UINT						_vertexCount;
	shared_ptr<vector<UINT>>	_indices;
	UINT						_indexCount;
};