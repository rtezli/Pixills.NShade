#pragma once

#include "common.h"
#include "model.h"
#include "material.h"

struct SceneFile
{

};

class Scene
{
public:
	Scene(DeviceResources *resources);
	~Scene();
	HRESULT AddModel(Model *model);
	HRESULT Load(wstring fileName);
	HRESULT Render();
public:
	const XMFLOAT3* AmbientColor;

	vector<Light>*			Lights()			{ return _lights.get(); }
	vector<Model>*			Models()			{ return _models.get(); }
	vector<Material>*		Materials()			{ return _materials.get(); }
	vector<NVertex>*		Vertices()			{ return _vertices.get(); }
	vector<UINT>*			Indices()			{ return _indices.get(); }
private:
	shared_ptr<vector<Light>>			_lights;
	shared_ptr<vector<Model>>			_models;
	shared_ptr<vector<ShaderSet>>		_shaders;
	shared_ptr<vector<Material>>		_materials;
	shared_ptr<vector<NVertex>>			_vertices;
	shared_ptr<vector<UINT>>	_indices;

	DeviceResources*					_deviceResources;
};

