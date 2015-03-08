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
	Scene(DeviceResources *resources);

	VOID AddModel(Model* model);
	VOID AddLight(Light* light);
	VOID AddCamera(Camera* camera);
	VOID Load(wstring fileName);
	VOID Clear();
	VOID Render();
public:
	Camera*					const GetCamera()		{ return _camera.get(); }
	Material*				const GetMaterials()	{ return _material.get(); }

	vector<Light>*			const GetLights()		{ return _lights.get(); }
	vector<Model>*			const GetModels()		{ return _models.get(); }
	vector<NVertex>*		const GetVertices()		{ return _vertices.get(); }
	vector<unsigned int>*	const GetIndices()		{ return _indices.get(); }
	
	static Scene*			CreateStandardScene(DeviceResources *pResources);
private:
	shared_ptr<Camera>					_camera;
	shared_ptr<Material>				_material;
	shared_ptr<vector<Light>>			_lights;
	shared_ptr<vector<Model>>			_models;
	shared_ptr<vector<Shader>>			_shaders;
	shared_ptr<vector<NVertex>>			_vertices;
	shared_ptr<vector<unsigned int>>	_indices;
	DeviceResources*					_resources;
};

