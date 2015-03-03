#pragma once

#include "includes.h"
#include "model.h"
#include "material.h"
#include "phongvertexshader.h"
#include "phongpixelshader.h"
#include "ambientlight.h"
#include "pointlight.h"

struct SceneFile
{

};

class Scene
{
public:
	Scene(DeviceResources* pResources);
	~Scene();
	HRESULT AddModel(Model* pModel);
	HRESULT AddLight(Light* pLight);
	HRESULT Load(wstring fileName);
	HRESULT Clear();
	HRESULT Render();
public:
	vector<Light>*			const GetLights()		{ return m_Lights.get();   }
	vector<Model>*			const GetModels()		{ return m_Models.get();   }
	Material*				const GetMaterials()	{ return m_Material.get(); }
	vector<nshade::Vertex>*	const GetVertices()		{ return m_Vertices.get(); }
	vector<unsigned int>*	const GetIndices()		{ return m_Indices.get();  }
	
	static Scene*			CreateStandardScene(DeviceResources* pResources);
private:
	shared_ptr<vector<Light>>			m_Lights;
	shared_ptr<vector<Model>>			m_Models;
	shared_ptr<vector<Shader>>			m_Shaders;
	shared_ptr<Material>				m_Material;
	shared_ptr<vector<nshade::Vertex>>	m_Vertices;
	shared_ptr<vector<unsigned int>>	m_Indices;
	DeviceResources*					m_pResources;
};

