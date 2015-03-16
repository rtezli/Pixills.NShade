#include "stdafx.h"
#include "scene.h"

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

        //// maybe we provide extra data here and write it to the registers instead of merging data hardcoded
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

        //// Check if this is neccessary if the pixel shader does not use the registers
        ////_resources->DeviceContext->PSSetConstantBuffers(0, 1, &cameraConstBuffer);
        Res::Get()->DeviceContext->PSSetShader(shaders->PixelShader->GetShader(), NULL, 0);
        Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }
}

void Scene::AddModel(Model *model)
{
    if (_models == NULL)
    {
        _models = shared_ptr<vector<Model>>(new vector<Model>());
    }
    _models->push_back(*model);
}

void Scene::AddLight(Light *light)
{
    if (_lights == NULL)
    {
        _lights = shared_ptr<vector<Light>>(new vector<Light>());
    }
    _lights->push_back(*light);
}

void Scene::AddCamera(Camera *camera)
{
    _camera = shared_ptr<Camera>(camera);
}

void Scene::Load(wstring fileName)
{
}

void Scene::SetAmbientLight(XMFLOAT4 *colorIntensity)
{
    _ambientLight = shared_ptr<XMFLOAT4>(colorIntensity);
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

    auto stdMaterial = new Material();
    stdMaterial->AssignShaders(shaders);

    auto stdModel = new Model();
    stdModel->LoadModelFromFBXFile("../Debug/teapot.fbx");
    stdModel->AssignMaterial(stdMaterial);
    scene->AddModel(stdModel);

    return new Scene();
}