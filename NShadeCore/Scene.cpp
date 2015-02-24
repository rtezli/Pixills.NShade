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
HRESULT Scene::Load(wstring fileName)
{
	return 0;
}