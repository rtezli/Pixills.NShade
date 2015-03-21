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

    auto camera = new Camera();
    camera->SetPosition(new XMFLOAT3(0.0f, 0.0f, 5.0f));
    camera->SetFocusPoint(new XMFLOAT3(0.0f, 0.0f, 0.0f));
    scene->AddCamera(camera);

    auto pointLightPosition = new XMFLOAT3(1.0f, 1.0f, 1.0f);
    auto pointLightColorIntensity = new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    auto pointLight = new PointLight();
    pointLight->SetPosition(pointLightPosition);
    pointLight->SetColorIntensity(pointLightColorIntensity);
    scene->AddLight(pointLight);

    /* teapot */
    auto flatVertexShader = VertexShader::Load(L"../Debug/FlatVertexShader.cso");
    auto phongPixelShader = PixelShader::Load(L"../Debug/PhongPixelShader.cso");

    auto tessHullShader = HullShader::Load(L"../Debug/TesselationHullShader.cso");
    auto tessDomainShader = DomainShader::Load(L"../Debug/TesselationDomainShader.cso");
    auto tessGeometryShader = GeometryShader::Load(L"../Debug/TesselationGeometryShader.cso");

    auto phongShaders = new Shaders();
    phongShaders->PixelShader = phongPixelShader;
    phongShaders->VertexShader = flatVertexShader;

    auto phongMaterial = new Material();
    phongMaterial->SetColor(new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    
    flatVertexShader->AddBuffer(camera->GetMatrixBuffer());
    flatVertexShader->AddBuffer(phongMaterial->GetColorBuffer());
    flatVertexShader->AddBuffer(scene->GetAmbientBuffer());
    flatVertexShader->AddBuffer(pointLight->GetBuffer());

    phongMaterial->AssignShaders(phongShaders);

    auto teapot = Model::LoadModelFromFBXFile("../Debug/teapot.fbx");
    teapot->AssignMaterial(phongMaterial);
    scene->AddModel(teapot);

    /* plane */

    auto textureVertexShader = VertexShader::Load(L"../Debug/TextureVertexShader.cso");
    textureVertexShader->AddBuffer(camera->GetMatrixBuffer());
    

    auto textureMaterial = new Material();
    textureMaterial->SetColor(new XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));

    auto gridTexture = Texture::Load(L"../Textures/grid_texture.dds");

    auto texturePixelShader = PixelShader::Load(L"../Debug/TexturePixelShader.cso");
    texturePixelShader->AddResource(gridTexture->GetResources());
    textureVertexShader->AddResource(gridTexture->GetResources());

    auto textureShaders = new Shaders();
    textureShaders->PixelShader = texturePixelShader;
    textureShaders->VertexShader = textureVertexShader;
    textureMaterial->AssignShaders(textureShaders);
    
    auto plane = Model::CreateHorizontalPlane(20.0f, new XMFLOAT3(0.0f, 0.0f, 0.0f));
    plane->AssignMaterial(textureMaterial);

    scene->AddModel(plane);

    return scene;
}