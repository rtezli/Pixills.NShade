#include "stdafx.h"

#include "scene.h"


Scene::Scene(DeviceResources* pResources)
{
	m_pResources = pResources;

	m_Lights = shared_ptr<vector<Light>>();
	m_Models = shared_ptr<vector<Model>>();
	m_Shaders = shared_ptr<vector<Shader>>();
	m_Material = shared_ptr<Material>();
	m_Vertices = shared_ptr<vector<nshade::Vertex>>();
	m_Indices = shared_ptr<vector<unsigned int>>();
}

Scene::~Scene()
{

}

HRESULT Scene::Clear()
{
	//GetDeviceContext()->UpdateSubresource(GetResources()->ConstBuffer, 0, nullptr, GetResources()->CameraConstBufferData, 0, 0);
	//GetDeviceContext()->OMSetRenderTargets(1, &GetResources()->RenderTargetView, GetResources()->DepthStencilView);
	//const FLOAT color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GetDeviceContext()->ClearRenderTargetView(GetResources()->RenderTargetView, color);
	//GetDeviceContext()->ClearDepthStencilView(GetResources()->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	return 0;
}

HRESULT Scene::Render()
{
	//auto models = GetScene()->Models();

	//for (unsigned int i = 0; i < models->size(); i++)
	//{
	//	auto model = models->at(i);
	//	model.Render();
	//	// UINT stride = sizeof(PhongShader::InputLayout);
	//	// UINT offset = 0;
	//	// auto vertices = model.Vertices();
	//	// auto indices = model.Indices();
	//	// auto material = model.Material();

	//	// GetDeviceContext()->IASetInputLayout(GetResources()->InputLayout);
	//	// GetDeviceContext()->IASetVertexBuffers(0, 1, &vertices, &stride, &offset);
	//	// GetDeviceContext()->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);
	//	// GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	// GetDeviceContext()->VSSetConstantBuffers(0, 1, &GetResources()->ConstBuffer);
	//	// GetDeviceContext()->VSSetShader(GetScene()->Materials()->Shaders() ->VertexShader, nullptr, 0);

	//	// GetDeviceContext()->PSSetConstantBuffers(0, 1, &GetResources()->ConstBuffer);
	//	// GetDeviceContext()->PSSetShader(GetResources()->Shaders->PixelShader, nullptr, 0);
	//}
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
	auto shaders = new vector<Shader>();

	auto stdPixelShader = new PhongShader::PhongPixelShader(pResources);
	auto stdVertexShader = new PhongShader::PhongVertexShader(pResources);

	shaders->push_back(*stdPixelShader);
	shaders->push_back(*stdVertexShader);

	auto stdMaterial = new Material();
	stdMaterial->Shaders = shared_ptr<vector<Shader>>(shaders);

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