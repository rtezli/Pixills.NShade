#include "stdafx.h"

#include "scene.h"

HRESULT Scene::Render()
{
    return 0;
}

HRESULT Scene::AddModel(Model *model)
{
    if (_models == NULL)
    {
        _models = make_shared<vector<Model>>();
    }
    GetModels()->push_back(*model);
    return 0;
}

HRESULT Scene::AddLight(Light *light)
{
    if (_models == NULL)
    {
        _lights = make_shared<vector<Light>>();
    }
    GetLights()->push_back(*light);
    return 0;
}

HRESULT Scene::Load(wstring fileName)
{
    return 0;
}