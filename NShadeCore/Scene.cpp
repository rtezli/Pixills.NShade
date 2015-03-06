#include "stdafx.h"

#include "scene.h"


Scene::Scene(DeviceResources* pResources)
{
	m_pResources = pResources;

	m_pMaterial = shared_ptr<Material>();
	m_pCamera = shared_ptr<Camera>();

	auto lights = new vector<Light>();
	m_pLights = shared_ptr<vector<Light>>(lights);

	auto models = new vector<Model>();
	m_pModels = shared_ptr<vector<Model>>(models);

	auto shaders = new vector<Shader>();
	m_pShaders = shared_ptr<vector<Shader>>(shaders);

	auto vertices = new vector<nshade::Vertex>();
	m_pVertices = shared_ptr<vector<nshade::Vertex>>(vertices);

	auto indices = new vector<unsigned int> ();
	m_pIndices = shared_ptr<vector<unsigned int>>(indices);
}

Scene::~Scene()
{

}

void Scene::Clear()
{
	auto constBuffer = GetCamera()->GetConstBufferData();
	m_pResources->DeviceContext->UpdateSubresource(GetCamera()->GetConstBuffer(), 0, nullptr, &constBuffer, 0, 0);
}

void Scene::Render()
{
	auto models = GetModels();

	for (unsigned int m = 0; m < models->size(); m++)
	{
		auto model = models->at(m);
		auto material = model.GetMaterial();

		auto shaders = model.GetMaterial()->GetShaders();
		auto vertexShader = shaders->VertexShader;
		auto layout = shaders->VertexShader->GetInputLayout();

		auto constBuffer = GetCamera()->GetConstBuffer();
		auto vertexBuffer = vertexShader->GetVertexBuffer();
		auto indexBuffer = vertexShader->GetIndexBuffer();
		auto strides = vertexShader->GetInputSize();
		UINT offset = 0;

		m_pResources->DeviceContext->IASetInputLayout(layout);
		m_pResources->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);

		m_pResources->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pResources->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pResources->DeviceContext->VSSetConstantBuffers(0, 1, &constBuffer);
		m_pResources->DeviceContext->VSSetShader(vertexShader->Shader(), nullptr, 0);

		// Check if this is neccessary if the pixel shader does not use the registers
		m_pResources->DeviceContext->PSSetConstantBuffers(0, 1, &constBuffer);
		m_pResources->DeviceContext->PSSetShader(shaders->PixelShader->Shader(), nullptr, 0);

		m_pResources->DeviceContext->DrawIndexed(model.GetIndices()->size(), 0, 0);
	}
}

void Scene::AddModel(Model* pModel)
{
	if (m_pModels == nullptr)
	{
		m_pModels = shared_ptr<vector<Model>>();
	}
	m_pModels->push_back(*pModel);
}

void Scene::AddLight(Light* pLight)
{
	if (m_pLights == nullptr)
	{
		m_pLights = shared_ptr<vector<Light>>();
	}
	m_pLights->push_back(*pLight);
}

void Scene::AddCamera(Camera* pCamera)
{
	m_pCamera = shared_ptr<Camera>(pCamera);
}

void Scene::Load(wstring fileName)
{
}

Scene* Scene::CreateStandardScene(DeviceResources* pResources)
{
	auto scene = new Scene(pResources);

	auto stdCamera = new Camera(pResources);
	stdCamera->SetPosition(new XMFLOAT3(0.0f, 0.0f, 5.0f));
	stdCamera->SetFocusPoint(new XMFLOAT3(0.0f, 0.0f, 0.0f));
	scene->AddCamera(stdCamera);


	auto ambientLightColor = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto ambientLightIntensity = 1.0f;
	auto stdAmbientLight = new AmbientLight(ambientLightColor, &ambientLightIntensity);
	scene->AddLight(stdAmbientLight);

	auto pointLightPosition = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto pointLightColor = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto pointLightIntensity = 1.0f;
	auto stdPointLight = new PointLight(pResources, pointLightPosition, pointLightColor, &pointLightIntensity);
	scene->AddLight(stdPointLight);

	//auto stdPixelShader = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/PhongPixelShader.cso");
	//auto stdVertexShader = ResourceManager::Current->MainResourceMap->GetSubtree("Files")->GetValue("Assets/PhongVertexShader.cso");

	auto stdPixelShader = new PhongShader::PhongPixelShader("../Debug/PhongPixelShader.cso", pResources);
	auto stdVertexShader = new PhongShader::PhongVertexShader("../Debug/PhongVertexShader.cso", pResources);
	
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