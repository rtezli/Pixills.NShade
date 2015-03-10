#include "stdafx.h"

#include "scene.h"

Scene::Scene(DeviceResources *pResources)
{
	_resources = pResources;

	_material = shared_ptr<Material>();
	_camera = shared_ptr<Camera>();

	auto lights = new vector<Light>();
	_lights = shared_ptr<vector<Light>>(lights);

	auto models = new vector<Model>();
	_models = shared_ptr<vector<Model>>(models);

	auto shaders = new vector<Shader>();
	_shaders = shared_ptr<vector<Shader>>(shaders);

	auto vertices = new vector<NVertex>();
	_vertices = shared_ptr<vector<NVertex>>(vertices);

	auto indices = new vector<UINT>();
	_indices = shared_ptr<vector<UINT>>(indices);
}

VOID Scene::Clear()
{
	auto constBuffer = GetCamera()->GetConstBufferData();
	_resources->DeviceContext->UpdateSubresource(GetCamera()->GetConstBuffer(), 0, NULL, &constBuffer, 0, 0);
}

VOID Scene::Render()
{
	auto models = GetModels();

	for (UINT m = 0; m < models->size(); m++)
	{
		auto model = models->at(m);
		auto material = model.GetMaterial();

		auto shaders = model.GetMaterial()->GetShaders();
		auto vertexShader = shaders->VertexShader;
		auto layout = shaders->VertexShader->GetInputLayout();

		auto camera = GetCamera();
		auto cameraConstBuffer = camera->GetConstBuffer();
		auto cameraPositionBuffer = camera->GetPositionBuffer();
		auto lights = GetLights();
		auto ambientLightConstBuffer = GetCamera()->GetConstBuffer();

		auto vertexBuffer = vertexShader->GetVertexBuffer();
		auto indexBuffer = vertexShader->GetIndexBuffer();
		auto strides = vertexShader->GetInputSize();
		UINT offset = 0;


		_resources->DeviceContext->IASetInputLayout(layout);
		_resources->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
		_resources->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		_resources->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// maybe we provide extra data here and write it to the registers instead of merging data hardcoded
		_resources->DeviceContext->VSSetConstantBuffers(0, 1, &cameraConstBuffer);
		_resources->DeviceContext->VSSetConstantBuffers(1, 1, &cameraPositionBuffer);

		for (UINT i = 0; i < lights->size(); i++)
		{
			auto light = lights->at(i);
			auto buffer = light.GetBuffer();
			_resources->DeviceContext->VSSetConstantBuffers(i + 2, 1, &buffer);
		}

		_resources->DeviceContext->VSSetShader(vertexShader->Shader(), NULL, 0);

		// Check if this is neccessary if the pixel shader does not use the registers
		_resources->DeviceContext->PSSetConstantBuffers(0, 1, &cameraConstBuffer);
		_resources->DeviceContext->PSSetShader(shaders->PixelShader->Shader(), NULL, 0);

		_resources->DeviceContext->DrawIndexed(model.GetIndices()->size(), 0, 0);
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

Scene* Scene::CreateStandardScene(DeviceResources *deviceResources)
{
	auto scene = new Scene(deviceResources);

	auto stdCamera = new Camera(deviceResources);
	stdCamera->SetPosition(new XMFLOAT3(0.0f, 0.0f, 5.0f));
	stdCamera->SetFocusPoint(new XMFLOAT3(0.0f, 0.0f, 0.0f));
	scene->AddCamera(stdCamera);

	auto ambientLightColor = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto ambientLightIntensity = 1.0f;
	auto stdAmbientLight = new AmbientLight(deviceResources, ambientLightColor, &ambientLightIntensity);
	scene->AddLight(stdAmbientLight);

	auto pointLightPosition = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto pointLightColor = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto pointLightIntensity = 1.0f;
	auto stdPointLight = new PointLight(deviceResources, pointLightPosition, pointLightColor, &pointLightIntensity);
	scene->AddLight(stdPointLight);

	//auto stdPixelShader = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/PhongPixelShader.cso");
	//auto stdVertexShader = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/PhongVertexShader.cso");

	auto stdPixelShader = new PhongShader::PhongPixelShader("../Debug/PhongPixelShader.cso", deviceResources);
	auto stdVertexShader = new PhongShader::PhongVertexShader("../Debug/PhongVertexShader.cso", deviceResources);

	auto stdMaterial = new Material();
	auto shaderSet = new ShaderSet();

	shaderSet->PixelShader = stdPixelShader;
	shaderSet->VertexShader = stdVertexShader;
	stdMaterial->Shaders = shared_ptr<ShaderSet>(shaderSet);

	//auto stdModelFile = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/teapot.fbx");

	auto stdModel = new Model();
	stdModel->LoadModelFromFBXFile("../Debug/teapot.fbx");
	stdModel->AssignMaterial(stdMaterial);
	stdModel->CreateBuffers();
	scene->AddModel(stdModel);

	return scene;
}