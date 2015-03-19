#include "stdafx.h"
#include "scene.h"

Scene::Scene()
{
    _models = shared_ptr<vector<Model>>(new vector<Model>());
    _lights = shared_ptr<vector<Light>>(new vector<Light>());
    _resourceMappings = shared_ptr<vector<ResourceMapping>>(new vector<ResourceMapping>());
}

void Scene::Clear()
{

}

void Scene::Render()
{

}

void Scene::AddModel(Model *model)
{
    _models->push_back(*model);
}

void Scene::AddLight(Light *light)
{
    _lights->push_back(*light);
}

void Scene::AddCamera(Camera *camera)
{
    _camera = shared_ptr<Camera>(camera);
}

void Scene::AddResourceMapping(ResourceMapping *mapping)
{
    _resourceMappings->push_back(*mapping);
}

void Scene::Load(wstring fileName)
{
}

void Scene::SetAmbientLight(XMFLOAT4 *colorIntensity)
{
    D3D11_BUFFER_DESC ambientBufferDesc = { 0 };
    ambientBufferDesc.ByteWidth = sizeof(XMFLOAT4);
    ambientBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA  ambientBufferData = { 0 };
    ambientBufferData.pSysMem = colorIntensity;
    ambientBufferData.SysMemPitch = 0;
    ambientBufferData.SysMemSlicePitch = 0;

    Res::Get()->Device->CreateBuffer(&ambientBufferDesc, &ambientBufferData, &_ambientBuffer);
}

Scene* Scene::CreateStandardScene()
{
    auto scene = new Scene();
    scene->SetAmbientLight(new XMFLOAT4(1.0f, 1.0f, 1.0f, 0.1f));

    auto stdCamera = new Camera();
    stdCamera->SetPosition(new XMFLOAT3(0.0f, 0.0f, 5.0f));
    stdCamera->SetFocusPoint(new XMFLOAT3(0.0f, 0.0f, 0.0f));
    scene->AddCamera(stdCamera);

    auto pointLightPosition = new XMFLOAT3(1.0f, 1.0f, 1.0f);
    auto pointLightColorIntensity = new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    auto stdPointLight = new PointLight();
    stdPointLight->SetPosition(pointLightPosition);
    stdPointLight->SetColorIntensity(pointLightColorIntensity);
    scene->AddLight(stdPointLight);

    auto stdPixelShader = PixelShader::Load(L"../Debug/PhongPixelShader.cso");
    auto stdVertexShader = VertexShader::Load(L"../Debug/PhongVertexShader.cso");

    auto stdHullShader = HullShader::Load(L"../Debug/TesselationHullShader.cso");
    auto stdDomainShader = DomainShader::Load(L"../Debug/TesselationDomainShader.cso");

    auto shaders = new Shaders();
    shaders->PixelShader = stdPixelShader;
    shaders->VertexShader = stdVertexShader;
    shaders->HullShader = stdHullShader;
    shaders->DomainShader = stdDomainShader;

    auto phongMaterial1 = new Material();
    phongMaterial1->AssignShaders(shaders);
    phongMaterial1->SetColor(new XMFLOAT4(0.1f, 0.1f, 0.6f, 1.0f));

    auto phongMaterial2 = new Material();
    auto gridTexture = Texture::Load(L"../Textures/grid_texture.dds");
    phongMaterial2->AssignTexture(gridTexture);
    phongMaterial2->AssignShaders(shaders);
    phongMaterial2->SetColor(new XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));

    auto stdTeapot = Model::LoadModelFromFBXFile("../Debug/teapot.fbx");
    stdTeapot->AssignMaterial(phongMaterial1);
    scene->AddModel(stdTeapot);

    auto stdPlane = Model::CreateHorizontalPlane(20.0f, new XMFLOAT3(0.0f, 0.0f, 0.0f));
    stdPlane->AssignMaterial(phongMaterial2);
    scene->AddModel(stdPlane);



    auto cameraMatrixBuffer = stdCamera->GetMatrixBuffer();
    auto cameraPositionBuffer = stdCamera->GetPositionBuffer();
    auto sceneAmbientBuffer = scene->GetAmbientBuffer();

    ResourceMapping m1 = { cameraMatrixBuffer, shaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m1);

    ResourceMapping m2 = { cameraPositionBuffer, shaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m2);

    ResourceMapping m3 = { sceneAmbientBuffer, shaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m3);

    ResourceMapping m4 = { stdPointLight->GetBuffer(), shaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m4);

    return scene;
}