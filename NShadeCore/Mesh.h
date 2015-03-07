#pragma once

#include "common.h"
#include "model.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
public:
	HRESULT AddModel(Model *model);
	HRESULT RemoveModel(Model *model);
	HRESULT Move(XMVECTOR *vector);
	HRESULT Rotate(XMVECTOR *vector);
public:
	shared_ptr<vector<Model>> m_Models;

};

