#include "stdafx.h"
#include "model.h"

Model::Model(DeviceResources* resources)
{
	m_pDeviceResources = shared_ptr<DeviceResources>(resources);
}

Model::~Model()
{

}

HRESULT Model::Initialize()
{
	auto result = LoadModelFromFBXFile("../Models/cube.fbx");
	if (FAILED(result))
	{
		return result;
	}

	//auto result = InitializeVertexBuffer();
	//if (FAILED(result))
	//{
	//	return result;
	//}

	//result = InitializeIndexBuffer(NULL);
	//if (FAILED(result))
	//{
	//	return result;
	//}

	return InitializeConstantBuffer();
}

HRESULT Model::LoadModelFromFBXFile(char* fileName)
{
	Debug::WriteCurrentDir();
	auto sdkManager = FbxManager::Create();

	auto fbxIOsettings = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(fbxIOsettings);

	fbxIOsettings->SetBoolProp(IMP_FBX_MATERIAL, false);
	fbxIOsettings->SetBoolProp(IMP_FBX_TEXTURE, false);
	fbxIOsettings->SetBoolProp(IMP_FBX_ANIMATION, false);
	fbxIOsettings->SetBoolProp(IMP_FBX_TEXTURE, false);

	auto fbxImporter = FbxImporter::Create(sdkManager, "");
	auto fbxScene = FbxScene::Create(sdkManager, "");

	auto success = fbxImporter->Initialize(fileName, -1, sdkManager->GetIOSettings());
	if (!success)
	{
		return success;
	}

	success = fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	if (!success)
	{
		return success;
	}

	auto axisSystem = fbxScene->GetGlobalSettings().GetAxisSystem();
	auto fbxRootNode = fbxScene->GetRootNode();

	auto rootNodeAttribute = fbxRootNode->GetNodeAttribute();
	FbxNodeAttribute::EType rootNodeTypeName = rootNodeAttribute == NULL ? FbxNodeAttribute::eUnknown : rootNodeAttribute->GetAttributeType();

	auto mesh = new vector<FbxNode*>();
	TraverseChildren(fbxRootNode, mesh);
	return TraverseAndStoreFbxNode1(mesh, &axisSystem);
}

HRESULT Model::TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh)
{
	auto count = node->GetChildCount();
	for (auto s = 0; s < count; s++)
	{
		node = node->GetChild(s);
		count = node->GetChildCount();
		auto nodeAttribute = node->GetNodeAttribute();
		FbxNodeAttribute::EType nodeType = nodeAttribute == NULL
			? FbxNodeAttribute::eUnknown
			: nodeAttribute->GetAttributeType();

		if (nodeType == FbxNodeAttribute::eMesh)
		{
			mesh->push_back(node);
		}
		else
		{
			for (auto c = 0; c < count; c++)
			{
				node = node->GetChild(s);
				nodeAttribute = node->GetNodeAttribute();
				nodeType = nodeAttribute == NULL
					? FbxNodeAttribute::eUnknown
					: nodeAttribute->GetAttributeType();

				if (nodeType == FbxNodeAttribute::eMesh)
				{
					mesh->push_back(node);
				}
			}
		}
	}
	return 0;
}

HRESULT Model::TraverseAndStoreFbxNode1(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem)
{
	auto count = nodes->size();

	auto modelVertices = new vector<Vertex>();
	auto modelIndexes = new vector<unsigned int>();

	// The scene maybe
	for (auto i = 0; i < count; i++)
	{
		auto child = nodes->at(i);
		auto childMesh = child->GetMesh();
		auto wipeMode = childMesh->GetWipeMode();
		auto polygonCount = childMesh->GetPolygonCount();

		 auto controlPoints = childMesh->GetControlPoints();
		 auto controlPointsCount = childMesh->GetControlPointsCount();
		 for (auto cp = 0; cp < controlPointsCount; cp++)
		 {
		 	auto point = controlPoints[cp];
		 	auto newVertex = new Vertex();
		 	newVertex->Position = ConvertFbxVector4ToXMFLOAT3(&point, axisSystem, 1.0f);
		 	modelVertices->push_back(*newVertex);
		 }

		//For each polygon in the model
		for (auto p = 0; p < polygonCount; p++)
		{
			auto startIndex = childMesh->GetPolygonVertexIndex(p);
			auto polygonVertices = childMesh->GetPolygonVertices();

			//For each point in a polygon get :  cooradinates, normals and index
			auto endIndex = startIndex + childMesh->GetPolygonSize(p);
			for (auto v = startIndex; v < endIndex; v++)
			{
				auto vertexIndex = polygonVertices[v];
				auto newVertex = &modelVertices->at(vertexIndex);
				auto point = childMesh->GetControlPointAt(vertexIndex);

				FbxVector4 normal;
				childMesh->GetPolygonVertexNormal(p, v, normal);

				newVertex->Color = XMFLOAT3
				{
					0.9f, 0.7f, 1.0f
				};

				newVertex->Normal = XMFLOAT3
				{
					static_cast<float>(normal.mData[0]),
					static_cast<float>(normal.mData[1]),
					static_cast<float>(normal.mData[2])
				};

				newVertex->UV = XMFLOAT2
				{
					0.0f, 0.0f
				};

				modelIndexes->push_back(vertexIndex);
			}
		}
	}

	return FillVertexAndIndexBuffer(modelVertices, modelIndexes);
}

HRESULT Model::TraverseAndStoreFbxNode2(FbxNode* fbxRootNode, FbxAxisSystem* axisSystem)
{
	auto count = fbxRootNode->GetChildCount();

	auto modelVertices = new vector<Vertex>();
	auto modelIndexes = new vector<unsigned int>();

	for (auto s = 0; s < count; s++)
	{
		auto child = fbxRootNode->GetChild(s);
		auto childName = child->GetName();
		auto childCount = child->GetChildCount();

		// For each model in the scene
		for (auto m = 0; m < childCount; m++)
		{
			child = child->GetChild(m);

			if (child->GetNodeAttribute() == NULL)
				continue;

			auto attributeType = child->GetNodeAttribute()->GetAttributeType();

			if (attributeType != FbxNodeAttribute::eMesh)
				continue;

			Debug::WriteLine("FBX : Loading child", childName);

			auto childMesh = child->GetMesh();
			auto vertexCount = childMesh->GetPolygonVertexCount();
			auto vertexIndexes = childMesh->GetPolygonVertices();
			auto controlPoints = childMesh->GetControlPoints();
			auto controlPointsCount = childMesh->GetControlPointsCount();

			for (auto cp = 0; cp < controlPointsCount; cp++)
			{
				auto point = controlPoints[cp];
				auto newVertex = new Vertex();

				newVertex->Position = ConvertFbxVector4ToXMFLOAT3(&point, axisSystem, 1.0);
				newVertex->Color = XMFLOAT3{ 0.9f, 0.7f, 1.0f };
				newVertex->UV = XMFLOAT2{ 0.0f, 0.0f };
				newVertex->Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };

				modelVertices->push_back(*newVertex);
			}

			for (auto v = 0; v < vertexCount; v++)
			{
				modelIndexes->push_back(vertexIndexes[v]);
			}
		}
	}

	return FillVertexAndIndexBuffer(modelVertices, modelIndexes);
}

HRESULT Model::FillVertexAndIndexBuffer(vector<Vertex>* modelVertices, vector<unsigned int>* modelIndexes)
{
	DeviceResource()->IndexCount = modelIndexes->size();
	DeviceResource()->VertexCount = modelVertices->size();

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * modelVertices->size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = modelVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	auto result = DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	DeviceResource()->IndexCount = modelIndexes->size();

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * DeviceResource()->IndexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = &modelIndexes[0];
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	// fbxScene->Destroy();
	// fbxRootNode->Destroy();

	return DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &DeviceResource()->IndexBuffer);
}

HRESULT Model::LoadModelFromOBJFile(char* fileName)
{
	// TODO : Get the vertices from the file

	Vertex* vertices[] = { 0 };

	m_initData.pSysMem = vertices;
	m_initData.SysMemPitch = 0;
	m_initData.SysMemSlicePitch = 0;

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	return 0;
}

XMFLOAT3 Model::ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, float scale)
{
	bool rightHanded = true;
	auto coordSystem = axisSystem->GetCoorSystem();
	if (coordSystem != FbxAxisSystem::ECoordSystem::eRightHanded)
	{
		rightHanded = false;
	}

	bool yUp = true;
	bool xFront = false;

	int upInverter = 1.0;
	int upVectorSign;
	auto upVector = axisSystem->GetUpVector(upVectorSign);
	if (upVectorSign != -1)
	{
		upInverter = -1.0;
	}
	if (upVector != FbxAxisSystem::eYAxis)
	{
		yUp = false;
	}

	int frontInverter = 1.0;
	int frontVectorSign;
	auto frontVector = axisSystem->GetFrontVector(frontVectorSign);
	if (frontVectorSign != -1)
	{
		frontInverter = -1.0;
	}

	auto x = 0.0f;
	auto y = 0.0f;
	auto z = 0.0f;

	XMFLOAT3 dxVector;

	if (xFront)
	{
		x = coordinate->mData[2] * scale;
		y = coordinate->mData[1] * upInverter * scale;
		z = coordinate->mData[0] * frontInverter * scale;
	}
	else
	{
		// Flip y and z to convert from RH to LH
		x = coordinate->mData[0] * scale;
		y = coordinate->mData[1] * scale;
		z = coordinate->mData[2] * -1.0 * scale;
	}

	dxVector = XMFLOAT3
	{
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(z)
	};
	return dxVector;
}

HRESULT Model::InitializeConstantBuffer()
{
	// Belongs to renderer class
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };
	constantBufferData.pSysMem = &DeviceResource()->ConstBuffer;
	constantBufferData.SysMemPitch = 0;
	constantBufferData.SysMemSlicePitch = 0;

	return DeviceResource()->Device->CreateBuffer(&constantBufferDesc, &constantBufferData, &DeviceResource()->ConstBuffer);
}

HRESULT Model::InitializeVertexBuffer()
{
	static const Vertex cube[] =
	{
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 1.0f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 1.0f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 1.0f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 1.0f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-15.0f, 0.0f, -15.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, 0.0f, -15.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, 0.0f, 15.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, 0.0f, 15.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(cube);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cube;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	return DeviceResource()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &DeviceResource()->VertexBuffer);
}

HRESULT Model::InitializeIndexBuffer(int indeces[])
{
	// 6 faces * 4 vertices = 24 vertices. 
	static const unsigned int cubeIndices[] =
	{
		0, 2, 1, 1, 2, 3, // Face 1 (2 Polygons)
		4, 5, 6, 5, 7, 6, // Face 2 (2 Polygons)
		0, 1, 5, 0, 5, 4, // Face 3 (2 Polygons)
		2, 6, 7, 2, 7, 3, // Face 4 (2 Polygons)
		0, 4, 6, 0, 6, 2, // Face 5 (2 Polygons)
		1, 3, 7, 1, 7, 5, // Face 6 (2 Polygons)

		8, 10, 11, 8, 9, 10 // F7 (The Plane)
	};

	DeviceResource()->IndexCount = ARRAYSIZE(cubeIndices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * ARRAYSIZE(cubeIndices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	auto result = DeviceResource()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &DeviceResource()->IndexBuffer);
	if (FAILED(result))
	{
		return result;
	}
	return result;
}

HRESULT Model::CreateVertexAndIndexBuffer(XMFLOAT3* vertices)
{
	//long size = 0;
	//int* indexBuffer = new int[size];
	//Vertex* vertexBuffer = new Vertex[size];

	//for (long i = 0; i < size; i++)
	//{
	//	vertexBuffer[i].Position = vertices[i];
	//	indexBuffer[i] = i;
	//}
	return 0;
}

// TODO : Add material