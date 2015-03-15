#include "stdafx.h"
#include "scene.h"

VOID Scene::Clear()
{
    //auto constBuffer = GetCamera()->GetConstBufferData();
    //Res::Get()->DeviceContext->UpdateSubresource(GetCamera()->GetConstBuffer(), 0, NULL, &constBuffer, 0, 0);
}

VOID Scene::Render()
{
    auto models = GetModels();

    for (UINT m = 0; m < models->size(); m++)
    {
        //auto model = models->at(m);
        //auto material = model.GetMaterial();

        //auto shaders = model.GetMaterial()->GetShaders();
        //auto layout  = shaders->VertexShader->GetInputLayout();

        //auto camera = GetCamera();
        //auto cameraConstBuffer = camera->GetConstBuffer();
        //auto cameraPositionBuffer = camera->GetPositionBuffer();
        //auto lights = GetLights();
        //auto ambientLightConstBuffer = GetCamera()->GetBuffer();

        //auto vertexShader = shaders->VertexShader;
        //auto vertexBuffer = vertexShader->GetVertexBuffer();
        //auto indexBuffer = vertexShader->GetIndexBuffer();
        //auto strides = vertexShader->GetInputSize();
        //UINT offset = 0;

        //Res::Get()->DeviceContext->IASetInputLayout(layout);
        //Res::Get()->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
        //Res::Get()->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        //Res::Get()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        //// maybe we provide extra data here and write it to the registers instead of merging data hardcoded
        //Res::Get()->DeviceContext->VSSetConstantBuffers(0, 1, &cameraConstBuffer);
        //Res::Get()->DeviceContext->VSSetConstantBuffers(1, 1, &cameraPositionBuffer);

        //for (UINT i = 0; i < lights->size(); i++)
        //{
        //    auto light = lights->at(i);
        //    auto buffer = light.GetBuffer();
        //    Res::Get()->DeviceContext->VSSetConstantBuffers(i + 2, 1, &buffer);
        //}

        //Res::Get()->DeviceContext->VSSetShader(vertexShader->Shader(), NULL, 0);

        //// Check if this is neccessary if the pixel shader does not use the registers
        ////_resources->DeviceContext->PSSetConstantBuffers(0, 1, &cameraConstBuffer);
        //Res::Get()->DeviceContext->PSSetShader(shaders->PixelShader->Shader(), NULL, 0);
        //Res::Get()->DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
    }
}

VOID Scene::AddModel(Model *model)
{
    if (_models == NULL)
    {
        _models = shared_ptr<vector<Model>>();
    }
    _models->push_back(*model);
}

VOID Scene::AddLight(Light *light)
{
    if (_lights == NULL)
    {
        _lights = shared_ptr<vector<Light>>();
    }
    _lights->push_back(*light);
}

VOID Scene::AddCamera(Camera *camera)
{
    _camera = shared_ptr<Camera>(camera);
}

VOID Scene::Load(wstring fileName)
{
}

Scene* Scene::CreateStandardScene()
{
    //auto scene = new Scene();

    //auto stdCamera = new Camera();
    //stdCamera->SetPosition(new XMFLOAT4(0.0f, 0.0f, 5.0f, 0.0f));
    //stdCamera->SetFocusPoint(new XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
    //scene->AddCamera(stdCamera);

    //auto ambientLightColor = new XMFLOAT3(1.0f, 1.0f, 1.0f);
    //auto ambientLightIntensity = 1.0f;
    //auto stdAmbientLight = new AmbientLight(deviceResources, ambientLightColor, &ambientLightIntensity);
    //scene->AddLight(stdAmbientLight);

    //auto pointLightPosition = new XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
    //auto pointLightColor = new XMFLOAT3(1.0f, 1.0f, 1.0f);
    //auto pointLightIntensity = 1.0f;
    //auto stdPointLight = new PointLight(deviceResources, pointLightPosition, pointLightColor, &pointLightIntensity);
    //scene->AddLight(stdPointLight);

    ////auto stdPixelShader = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/PhongPixelShader.cso");
    ////auto stdVertexShader = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/PhongVertexShader.cso");

    //auto stdPixelShader = new PhongShader::PhongPixelShader("../Debug/PhongPixelShader.cso", deviceResources);
    //auto stdVertexShader = new PhongShader::PhongVertexShader("../Debug/PhongVertexShader.cso", deviceResources);

    //auto stdMaterial = new Material();
    //auto shaderSet = new ShaderSet();

    //shaderSet->PixelShader = stdPixelShader;
    //shaderSet->VertexShader = stdVertexShader;
    //stdMaterial->Shaders = shared_ptr<ShaderSet>(shaderSet);

    ////auto stdModelFile = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/teapot.fbx");

    //auto stdModel = new Model();
    //stdModel->LoadModelFromFBXFile("../Debug/teapot.fbx");
    //stdModel->AssignMaterial(stdMaterial);
    //stdModel->CreateBuffers();
    //scene->AddModel(stdModel);

    return new Scene();
}