#include "stdafx.h"

#include "scene.h"


Scene::Scene(DeviceResources *resources)
{
	_deviceResources = resources;
}

Scene::~Scene()
{

}

HRESULT Scene::Render()
{
	return 0;
}

HRESULT Scene::AddModel(Model * model)
{
	if (_models == nullptr)
	{
		_models = make_shared<vector<Model>>();
	}
	Models()->push_back(*model);
	return 0;
}
HRESULT Scene::Load(wstring fileName)
{
	return 0;
}