#pragma once

#include "export.h"
#include "model.h"
#include "directxmath.h"
#include "memory"
#include "vector"

class Mesh
{
public:
	Mesh();
	~Mesh();
public:
	HRESULT AddModel(Model* model);
	HRESULT RemoveModel(Model* model);
	HRESULT Move(XMVECTOR* vector);
	HRESULT Rotate(XMVECTOR* vector);
public:
	std::shared_ptr<std::vector<Model>> m_Models;

};

