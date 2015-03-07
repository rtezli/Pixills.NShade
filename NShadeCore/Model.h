#pragma once

#include "common.h"
#include "export.h"
#include "material.h"

EXTERN class API Model
{
public:
	void LoadModelFromFBXFile(char* fileName);
	void LoadModelFromOBJFile(char* fileName, BOOL isRightHand);
	void AssignMaterial(Material* pMaterial);

	Material*					const GetMaterial(){ return m_pMaterial.get(); }
	vector<unsigned int>*		const GetIndices(){ return m_pIndices.get(); }
	vector<NVertex>*		const GetVertices(){ return m_pVertices.get(); }
	void								CreateBuffers();
private:	
	shared_ptr<Material>				m_pMaterial;
	shared_ptr<vector<NVertex>>	m_pVertices;
	shared_ptr<vector<unsigned int>>	m_pIndices;
};