#include "stdafx.h"

#include "scene.h"


Scene::Scene(DeviceResources* pResources)
{
	m_pResources = pResources;
}

Scene::~Scene()
{

}

HRESULT Scene::Render()
{
	//for_each(Models()->begin(), Models - end(), [](Model &model){ model->Render()});
	return 0;
}

HRESULT Scene::AddModel(Model* pModel)
{
	if (m_Models == nullptr)
	{
		m_Models = make_shared<vector<Model>>();
	}
	Models()->push_back(*pModel);
	return 0;
}

HRESULT Scene::AddLight(Light* pLight)
{
	if (m_Lights == nullptr)
	{
		m_Lights = make_shared<vector<Light>>();
	}
	Lights()->push_back(*pLight);
	return 0;
}

HRESULT Scene::Load(wstring fileName)
{
	return 0;
}

Scene* Scene::CreateStandardScene(DeviceResources* pResources)
{
	auto scene = new Scene(pResources);

	auto stdPixelShader = new PhongShader::PhongPixelShader(pResources);
	auto stdVertexShader = new PhongShader::PhongVertexShader(pResources);

	auto stdMaterial = new Material();
	stdMaterial->Shaders->push_back(*stdPixelShader);
	stdMaterial->Shaders->push_back(*stdVertexShader);

	auto stdModel = new Model(pResources);
	stdModel->LoadModelFromFBXFile("../Debug/teapot.fbx");
	stdModel->AssignMaterial(stdMaterial);

	scene->AddModel(stdModel);

	auto stdAmbientLight = new AmbientLight();
	//stdAmbientLight->Color = new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	scene->AddLight(stdAmbientLight);


	auto stdPointLight = new PointLight();
	//stdPointLight->Position = new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//stdPointLight->Color = new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	scene->AddLight(stdPointLight);

	return scene;
}