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
	auto models = GetModels();

	for (unsigned int m = 0; m < models->size(); m++)
	{
		auto model = models->at(m);
		auto material = model.GetMaterial();

		auto stride = 0;
		//ID3D11Buffer buffers[]{0};
		auto vertexBuffer = model.GetVertexBuffer();
		/* Input Assembler */
 
		m_pResources->DeviceContext->IASetInputLayout(m_pResources->InputLayout);
		//m_pResources->DeviceContext->IASetVertexBuffers(0, 1, &buffers, &stride, 0);
		m_pResources->DeviceContext->IASetIndexBuffer(model.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		m_pResources->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	return 0;
}

HRESULT Scene::AddModel(Model* pModel)
{
	if (m_Models == nullptr)
	{
		m_Models = shared_ptr<vector<Model>>();
	}
	m_Models->push_back(*pModel);
	return 0;
}

HRESULT Scene::AddLight(Light* pLight)
{
	if (m_Lights == nullptr)
	{
		m_Lights = shared_ptr<vector<Light>>();
	}
	m_Lights->push_back(*pLight);
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
	stdMaterial->Shaders->PixelShader = stdPixelShader;
	stdMaterial->Shaders->VertexShader = stdVertexShader;

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