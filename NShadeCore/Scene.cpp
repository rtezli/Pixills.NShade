#include "stdafx.h"

#include "Scene.h"


Scene::Scene(DeviceResources* pResources)
{
	m_pResources = pResources;
}

Scene::~Scene()
{

}

HRESULT Scene::Render()
{
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
