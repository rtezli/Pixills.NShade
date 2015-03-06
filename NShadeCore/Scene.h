#pragma once

#include "common.h"
#include "export.h"
#include "model.h"
#include "material.h"
#include "phongvertexshader.h"
#include "phongpixelshader.h"
#include "ambientlight.h"
#include "pointlight.h"
#include "camera.h"

struct SceneFile
{

};

EXTERN class API Scene
{
public:
	Scene(DeviceResources* pResources);
	~Scene();
	void AddModel(Model* pModel);
	void AddLight(Light* pLight);
	void AddCamera(Camera* pCamera);
	void Load(wstring fileName);
	void Clear();
	void Render();
public:
	Camera*					const GetCamera()		{ return m_pCamera.get(); }
	Material*				const GetMaterials()	{ return m_pMaterial.get(); }

	vector<Light>*			const GetLights()		{ return m_pLights.get(); }
	vector<Model>*			const GetModels()		{ return m_pModels.get(); }
	vector<nshade::Vertex>*	const GetVertices()		{ return m_pVertices.get(); }
	vector<unsigned int>*	const GetIndices()		{ return m_pIndices.get(); }
	
	static Scene*			CreateStandardScene(DeviceResources* pResources);
private:
	shared_ptr<Camera>					m_pCamera;
	shared_ptr<Material>				m_pMaterial;

	shared_ptr<vector<Light>>			m_pLights;
	shared_ptr<vector<Model>>			m_pModels;
	shared_ptr<vector<Shader>>			m_pShaders;
	shared_ptr<vector<nshade::Vertex>>	m_pVertices;
	shared_ptr<vector<unsigned int>>	m_pIndices;
	DeviceResources*					m_pResources;
};

