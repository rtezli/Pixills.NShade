#pragma once

#include "includes.h"
#include "model.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
public:
	HRESULT AddModel(Model* model);
	HRESULT RemoveModel(Model* model);
	HRESULT Move(DirectX::XMVECTOR* vector);
	HRESULT Rotate(DirectX::XMVECTOR* vector);
public:
	std::shared_ptr<std::vector<Model>> m_Models;

};

