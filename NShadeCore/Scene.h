#pragma once

//#include "common.h"
#include "vector"
#include "model.h"
#include "light.h"

using namespace std;

struct SceneFile
{

};

EXTERN class API Scene
{
public:
    HRESULT AddModel(Model *model);
    HRESULT	AddLight(Light *light);
    HRESULT Load(wstring fileName);
    HRESULT Render();
public:
    const XMFLOAT3* AmbientColor;

    vector<Light>*          GetLights(){ return _lights.get(); }
    vector<Model>*          GetModels(){ return _models.get(); }
private:
    shared_ptr<vector<Light>>   _lights;
    shared_ptr<vector<Model>>   _models;
    shared_ptr<Light>           _ambientLight;
};

