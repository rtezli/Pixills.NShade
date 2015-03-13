#pragma once

#include "common.h"
#include "model.h"

EXTERN class API Mesh
{
public:
	HRESULT AddModel(Model* model);
	HRESULT RemoveModel(Model* model);
	HRESULT Move(XMVECTOR* vector);
	HRESULT Rotate(XMVECTOR* vector);
public:
	shared_ptr<vector<Model>> _models;

};

