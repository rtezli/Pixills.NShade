#pragma once

#include "common.h"
#include "model.h"

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

    vector<Light>*          Lights(){ return _lights.get(); }
    vector<Model>*          Models(){ return _models.get(); }
private:
    shared_ptr<vector<Light>>   _lights;
    shared_ptr<vector<Model>>   _models;
    shared_ptr<Light>           _ambientLight;
};

