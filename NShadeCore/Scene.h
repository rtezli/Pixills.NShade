#pragma once

#include "includes.h"
#include "model.h"
#include "material.h"

class Scene
{
public:
	Scene(DeviceResources* pResources);
	~Scene();
	HRESULT Render();
	HRESULT Initialize();
public:
	XMFLOAT3* AmbientColor;
	shared_ptr<vector<Light>>			Lights;
	shared_ptr<vector<Model>>			Models;
	shared_ptr<vector<ShaderSet>>		Shaders;
	shared_ptr<vector<Material>>		Materials;
	shared_ptr<vector<nshade::Vertex>>	Vertices;
	vector<unsigned int>				Indices;
private:
	DeviceResources*					m_pResources;
};

