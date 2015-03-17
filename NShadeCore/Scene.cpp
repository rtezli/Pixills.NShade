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
    //auto constBuffer = GetCamera()->GetConstBufferData();
    //Res::Get()->DeviceContext->UpdateSubresource(GetCamera()->GetConstBuffer(), 0, NULL, &constBuffer, 0, 0);
}

void Scene::Render()
{
    auto models = GetModels();

    for (UINT m = 0; m < models->size(); m++)
    {
        auto model = models->at(m);
        auto material = model.GetMaterial();

        auto shaders = model.GetMaterial()->GetShaders();
        auto layout  = shaders->VertexShader->GetInputLayout();

        auto camera = GetCamera();
        auto cameraMatrixBuffer = camera->GetMatrixBuffer();
        auto cameraPositionBuffer = camera->GetPositionBuffer();
        auto sceneAmbientBuffer = GetAmbientBuffer();

        auto vertexShader = shaders->VertexShader;
        auto vertexBuffer = model.GetVertexBuffer();
        auto indexBuffer = model.GetIndexBuffer();
        auto strides = vertexShader->GetInputSize();
        UINT offset = 0;

        Res::Get()->DeviceContext->IASetInputLayout(layout);
        Res::Get()->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
        Res::Get()->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        Res::Get()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        Res::Get()->DeviceContext->VSSetConstantBuffers(0, 1, &cameraMatrixBuffer);
        Res::Get()->DeviceContext->VSSetConstantBuffers(1, 1, &cameraPositionBuffer);
        Res::Get()->DeviceContext->VSSetConstantBuffers(2, 1, &sceneAmbientBuffer);

        //auto lights = GetLights();
        //for (UINT i = 0; i < lights->size(); i++)
        //{
        //    auto light = lights->at(i);
        //    auto buffer = light.GetBuffer();
        //    Res::Get()->DeviceContext->VSSetConstantBuffers(i + 2, 1, &buffer);
        //}

        Res::Get()->DeviceContext->VSSetShader(vertexShader->GetShader(), NULL, 0);


        Res::Get()->DeviceContext->PSSetShader(shaders->PixelShader->GetShader(), NULL, 0);
        Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }
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

    auto shaders = new Shaders();
    shaders->PixelShader = stdPixelShader;
    shaders->VertexShader = stdVertexShader;

    auto phongMaterial1 = new Material();
    phongMaterial1->AssignShaders(shaders);
    phongMaterial1->SetColor(new XMFLOAT4(0.1f, 0.1f, 0.6f, 1.0f));

    auto phongMaterial2 = new Material();
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