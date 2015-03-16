#pragma once

#include "vector"
#include "memory"
#include "model.h"
#include "light.h"
#include "pointlight.h"
#include "camera.h"
#include "shader.h"

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
    void AddModel(Model* model);
    void AddLight(Light* light);
    void AddCamera(Camera* camera);
    void Load(wstring fileName);
    void Clear();
    void Render();
public:
    Camera*                     const GetCamera()           { return _camera.get(); }
    Material*                   const GetMaterials()        { return _material.get(); }
    vector<ResourceMapping>*    const GetMappings()         { return _resourceMappings.get(); }

    vector<Light>*              const GetLights()           { return _lights.get(); }
    vector<Model>*              const GetModels()           { return _models.get(); }
    vector<NVertex>*            const GetVertices()         { return _vertices.get(); }
    vector<unsigned int>*       const GetIndices()          { return _indices.get(); }
    ID3D11Buffer*               const GetAmbientBuffer()    { return _ambientBuffer.get(); }

    void                        SetAmbientLight(XMFLOAT4 *colorIntensity);

    static Scene*               CreateStandardScene();
private:
    shared_ptr<XMFLOAT4>                _ambientLight;
    shared_ptr<Camera>                  _camera;
    shared_ptr<Material>                _material;
    shared_ptr<vector<Light>>           _lights;
    shared_ptr<vector<Model>>           _models;
    shared_ptr<vector<Shader>>          _shaders;
    shared_ptr<vector<NVertex>>         _vertices;
    shared_ptr<vector<ResourceMapping>> _resourceMappings;
    shared_ptr<vector<unsigned int>>    _indices;
    shared_ptr<ID3D11Buffer>            _ambientBuffer;
};