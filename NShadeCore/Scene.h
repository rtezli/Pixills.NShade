#pragma once

#include "includes.h"
#include "model.h"
#include "material.h"

class Scene
{
public:
	Scene(DeviceResources* pResources);
	~Scene();
	HRESULT AddModel(Model* pModel);
	HRESULT Render();
public:
	const XMFLOAT3* AmbientColor;

	vector<Light>*			Lights()			{ return m_Lights.get(); }
	vector<Model>*			Models()			{ return m_Models.get(); }
	vector<ShaderSet>*		Shaders()			{ return m_Shaders.get(); }
	vector<Material>*		Materials()			{ return m_Materials.get(); }
	vector<nshade::Vertex>*	Vertices()			{ return m_Vertices.get(); }
	vector<unsigned int>*	Indices()			{ return m_Indices.get(); }
private:
	shared_ptr<vector<Light>>			m_Lights;
	shared_ptr<vector<Model>>			m_Models;
	shared_ptr<vector<ShaderSet>>		m_Shaders;
	shared_ptr<vector<Material>>		m_Materials;
	shared_ptr<vector<nshade::Vertex>>	m_Vertices;
	shared_ptr<vector<unsigned int>>	m_Indices;

	DeviceResources*					m_pResources;
};

