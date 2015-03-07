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
	Scene(DeviceResources *pResources);

	VOID AddModel(Model* pModel);
	VOID AddLight(Light* pLight);
	VOID AddCamera(Camera* pCamera);
	VOID Load(wstring fileName);
	VOID Clear();
	VOID Render();
public:
	Camera*					const GetCamera()		{ return m_pCamera.get(); }
	Material*				const GetMaterials()	{ return m_pMaterial.get(); }

	vector<Light>*			const GetLights()		{ return m_pLights.get(); }
	vector<Model>*			const GetModels()		{ return m_pModels.get(); }
	vector<NVertex>*		const GetVertices()		{ return m_pVertices.get(); }
	vector<unsigned int>*	const GetIndices()		{ return m_pIndices.get(); }
	
	static Scene*			CreateStandardScene(DeviceResources *pResources);
private:
	shared_ptr<Camera>					m_pCamera;
	shared_ptr<Material>				m_pMaterial;

	shared_ptr<vector<Light>>			m_pLights;
	shared_ptr<vector<Model>>			m_pModels;
	shared_ptr<vector<Shader>>			m_pShaders;
	shared_ptr<vector<NVertex>>	m_pVertices;
	shared_ptr<vector<unsigned int>>	m_pIndices;
	DeviceResources*					m_pResources;
};

