#pragma once

#include "common.h"
#include "export.h"
#include "material.h"

EXTERN class API Model
{
public:
	Model();
	~Model();
	void LoadModelFromFBXFile(char* fileName);
	void LoadModelFromOBJFile(char* fileName, bool isRightHand);
	void AssignMaterial(Material* pMaterial);

	Material*					const GetMaterial(){ return m_pMaterial.get(); }
	vector<unsigned int>*		const GetIndices(){ return m_pIndices.get(); }
	vector<nshade::Vertex>*		const GetVertices(){ return m_pVertices.get(); }
	void								CreateBuffers();
private:	
	shared_ptr<Material>				m_pMaterial;
	shared_ptr<vector<nshade::Vertex>>	m_pVertices;
	shared_ptr<vector<unsigned int>>	m_pIndices;
};