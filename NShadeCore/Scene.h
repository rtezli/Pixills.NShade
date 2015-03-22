#pragma once

#include "vector"
#include "memory"
#include "model.h"
#include "light.h"
#include "pointlight.h"
#include "camera.h"
#include "shaders.h"
#include "processingstep.h"

using namespace std;

struct SceneFile
{

};

struct ResourceMapping
{
    ID3D11Resource* Source;
    ID3D11Resource* Target;
};

EXTERN class API Scene
{
public:
    Scene();
public:
    void AddModel(Model *model);
    void AddLight(Light *light);
    void AddCamera(Camera *camera);
    void AddResourceMapping(ResourceMapping *mapping);
    void AddPostProcessingStep(ProcessingStep *step);
    void Load(wstring fileName);
    void Clear();
    void Render();
public:
    vector<ProcessingStep>*     const GetPostProcessingSteps()  { return _processingSteps.get(); }
    Camera*                     const GetCamera()               { return _camera.get(); }
    vector<ResourceMapping>*    const GetMappings()             { return _resourceMappings.get(); }

    vector<Light>*              const GetLights()               { return _lights.get(); }
    vector<Model>*              const GetModels()               { return _models.get(); }
    ID3D11Buffer*               const GetAmbientBuffer()        { return _ambientBuffer; }

    void                        SetAmbientLight(XMFLOAT4 *colorIntensity);

    static Scene*               CreateStandardScene();
private:
    shared_ptr<vector<ProcessingStep>>  _processingSteps;
    shared_ptr<Camera>                  _camera;
    shared_ptr<vector<Light>>           _lights;
    shared_ptr<vector<Model>>           _models;
    shared_ptr<vector<ResourceMapping>> _resourceMappings;
    ID3D11Buffer                        *_ambientBuffer;
};