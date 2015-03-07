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
	vector<NVertex>*	const GetVertices(){ return _vertices.get(); }
	void				CreateBuffers();
private:	
	shared_ptr<Material>		_material;
	shared_ptr<vector<NVertex>>	_vertices;
	shared_ptr<vector<UINT>>	_indices;
};