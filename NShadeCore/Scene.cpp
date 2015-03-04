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
		auto vertexBuffer = model.GetVertexBuffer();
		auto strides = model.GetVertexBufferStrides();
		auto shaders = model.GetMaterial()->GetShaders();
		auto constBuffer = GetCamera()->GetConstBuffer();

		m_pResources->DeviceContext->IASetInputLayout(shaders->VertexShader->GetInputLayout());
		m_pResources->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, 0);
		m_pResources->DeviceContext->IASetIndexBuffer(model.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		m_pResources->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pResources->DeviceContext->VSSetConstantBuffers(0, 1, &constBuffer);
		m_pResources->DeviceContext->VSSetShader(shaders->VertexShader->Shader(), nullptr, 0);

		// Check if this is neccessary if the pixel shader does not use the registers
		// m_pResources->DeviceContext->PSSetConstantBuffers(0, 1, &constBuffer);
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
	//stdCamera->Position = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	//stdCamera->LookAt = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	scene->AddCamera(stdCamera);

	auto stdAmbientLight = new AmbientLight();
	//stdAmbientLight->Color = new XMFLOAT3(1.0f, 1.0f, 1.0f, 1.0f);
	//stdAmbientLight->Intensity = 1.0f;
	scene->AddLight(stdAmbientLight);


	auto stdPointLight = new PointLight();
	//stdPointLight->Position = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	//stdPointLight->Color = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	//stdPointLight->Intensity = 1.0f;
	scene->AddLight(stdPointLight);

	auto stdPixelShader = new PhongShader::PhongPixelShader("../Debug/PhongPixelShader.cso", pResources);
	auto stdVertexShader = new PhongShader::PhongVertexShader("../Debug/PhongVertexShader.cso", pResources);
	
	auto stdMaterial = new Material();
	stdMaterial->Shaders->PixelShader = stdPixelShader;
	stdMaterial->Shaders->VertexShader = stdVertexShader;

	auto stdModel = new Model(pResources);
	stdModel->LoadModelFromFBXFile("../Debug/teapot.fbx");
	stdModel->AssignMaterial(stdMaterial);

	scene->AddModel(stdModel);

	return scene;
}

//HRESULT Renderer::Render()
//{
//	// Clear
//	ClearScene();
//
//	if (!m_isInitialized)
//	{
//		return 0;
//	}
//
//	UINT stride = sizeof(nshade::VertexShaderInput);
//	UINT offset = 0;
//
//	// Set model data
//	GetDeviceContext()->IASetInputLayout(Resources()->InputLayout);
//
//	// Set multiple buffers here ? i.e. each for one model since some shaders are not applied to all models
//	GetDeviceContext()->IASetVertexBuffers(0, 1, &Resources()->VertexBuffer, &stride, &offset);
//	GetDeviceContext()->IASetIndexBuffer(Resources()->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	// Set shader data
//
//
//	// Set multiple shaders here ?
//	auto vs = Resources()->Shaders->VertexShader;
//	auto ps = Resources()->Shaders->PixelShader;
//
//	GetDeviceContext()->VSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
//	GetDeviceContext()->VSSetShader(vs, nullptr, 0);
//
//	GetDeviceContext()->PSSetConstantBuffers(0, 1, &Resources()->ConstBuffer);
//	GetDeviceContext()->PSSetShader(ps, nullptr, 0);
//	GetDeviceContext()->DrawIndexed(Resources()->IndexCount, 0, 0);
//
//	// Present
//	return Resources()->SwapChain->Present(1, 0);
//}