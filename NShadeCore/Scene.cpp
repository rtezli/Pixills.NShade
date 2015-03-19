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

    /* teapot */
    auto phongPixelShader = PixelShader::Load(L"../Debug/PhongPixelShader.cso");
    auto phongVertexShader = VertexShader::Load(L"../Debug/PhongVertexShader.cso");

    auto phongShaders = new Shaders();
    phongShaders->PixelShader = phongPixelShader;
    phongShaders->VertexShader = phongVertexShader;

    auto phongMaterial = new Material();
    phongMaterial->AssignShaders(phongShaders);
    phongMaterial->SetColor(new XMFLOAT4(0.1f, 0.1f, 0.6f, 1.0f));

    auto teapot = Model::LoadModelFromFBXFile("../Debug/teapot.fbx");
    teapot->AssignMaterial(phongMaterial);
    scene->AddModel(teapot);

    /* plane */
    auto texturePixelShader = PixelShader::Load(L"../Debug/TexturePixelShader.cso");
    auto textureVertexShader = VertexShader::Load(L"../Debug/TextureVertexShader.cso");

    auto textureShaders = new Shaders();
    textureShaders->PixelShader = texturePixelShader;
    textureShaders->VertexShader = textureVertexShader;

    auto textureMaterial = new Material();
    auto gridTexture = Texture::Load(L"../Textures/grid_texture.dds");
    textureMaterial->AssignTexture(gridTexture);
    textureMaterial->AssignShaders(textureShaders);
    textureMaterial->SetColor(new XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));

    auto plane = Model::CreateHorizontalPlane(20.0f, new XMFLOAT3(0.0f, 0.0f, 0.0f));
    plane->AssignMaterial(textureMaterial);
    scene->AddModel(plane);

    auto cameraMatrixBuffer = stdCamera->GetMatrixBuffer();
    auto cameraPositionBuffer = stdCamera->GetPositionBuffer();
    auto sceneAmbientBuffer = scene->GetAmbientBuffer();

    ResourceMapping m1 = { cameraMatrixBuffer, phongShaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m1);

    ResourceMapping m2 = { cameraPositionBuffer, phongShaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m2);

    ResourceMapping m3 = { sceneAmbientBuffer, phongShaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m3);

    ResourceMapping m4 = { stdPointLight->GetBuffer(), phongShaders->PixelShader->GetInput() };
    scene->AddResourceMapping(&m4);

    return scene;
}