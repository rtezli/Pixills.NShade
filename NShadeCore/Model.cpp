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
	//auto result = LoadModelFromFBXFile("../Models/cube.fbx");
	//if (FAILED(result))
	//{
	//	return result;
	//}

	auto result = LoadModelFromOBJFile("../Models/teapot.obj", true);
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

	auto geometryConverter = new FbxGeometryConverter(sdkManager);

	auto success = geometryConverter->Triangulate(fbxScene, true);
	if (!success)
	{
		return success;
	}

	success = fbxImporter->Initialize(fileName, -1, sdkManager->GetIOSettings());
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

	auto modelVertices = new vector<nshade::Vertex>();
	auto modelIndecies = new vector<unsigned int>();

	// The scene maybe
	for (unsigned int i = 0; i < count; i++)
	{
		auto child = nodes->at(i);

		auto mesh = child->GetMesh();
		auto controlPoints = mesh->GetControlPoints();
		auto controlPointsCount = mesh->GetControlPointsCount();

		// Copy all control points
		for (auto cp = 0; cp < controlPointsCount; cp++)
		{
			auto point = controlPoints[cp];
			auto newVertex = new nshade::Vertex();
			// Set vertex position (and invert Z)
			newVertex->Position = ConvertFbxVector4ToXMFLOAT3(&point, axisSystem, 1.0f);
			modelVertices->push_back(*newVertex);
		}

		auto polygonCount = mesh->GetPolygonCount();

		//For each polygon in the model
		for (auto p = 0; p < polygonCount; p++)
		{
			auto polygonSize = mesh->GetPolygonSize(p);
			//For each point in a polygon get :  cooradinates, normals and index
			for (auto v = 0; v < polygonSize; v++)
			{
				auto vertexIndex = mesh->GetPolygonVertex(p, v);
				auto newVertex = &modelVertices->at(vertexIndex);

				// Create the normal
				FbxVector4 normal;
				mesh->GetPolygonVertexNormal(p, v, normal);
				newVertex->Normal = XMFLOAT3
				{
					static_cast<float>(normal.mData[0]),
					static_cast<float>(normal.mData[1]),
					static_cast<float>(normal.mData[2])
				};

				// Use a standard color for all vertices
				newVertex->Color = XMFLOAT3
				{
					0.9f, 0.7f, 1.0f
				};

				// Dont set that now
				newVertex->UV = XMFLOAT2
				{
					0.0f, 0.0f
				};

				modelIndecies->push_back(vertexIndex);
			}
		}
	}

	return FillVertexAndIndexBuffer(modelVertices, modelIndecies);
}

HRESULT Model::TraverseAndStoreFbxNode2(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem)
{
	auto count = nodes->size();

	auto modelVertices = new vector<nshade::Vertex>();
	auto modelIndexes = new vector<unsigned int>();

	// For each model in the scene
	for (unsigned int m = 0; m < count; m++)
	{
		auto child = nodes->at(m);
		auto childMesh = child->GetMesh();

		auto vertexCount = childMesh->GetPolygonVertexCount();
		auto vertexIndexes = childMesh->GetPolygonVertices();

		auto controlPoints = childMesh->GetControlPoints();
		auto controlPointsCount = childMesh->GetControlPointsCount();

		for (auto cp = 0; cp < controlPointsCount; cp++)
		{
			auto point = controlPoints[cp];
			auto newVertex = new nshade::Vertex();

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

	return FillVertexAndIndexBuffer(modelVertices, modelIndexes);
}

HRESULT Model::FillVertexAndIndexBuffer(vector<nshade::Vertex>* modelVertices, vector<unsigned int>* modelIndexes)
{
	DeviceResource()->IndexCount = modelIndexes->size();
	DeviceResource()->VertexCount = modelVertices->size();

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(nshade::Vertex) * modelVertices->size();
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

HRESULT Model::LoadModelFromOBJFile(char* fileName, bool isRightHand)
{
	auto fileLines = File::ReadFileLines(fileName);
	if (fileLines.empty())
	{
		return 0;
	}

	vector<string> parts;

	auto vertices = new vector<nshade::Vertex>();
	auto indices = new vector<unsigned int>();
	auto polygons = new vector<nshade::Polygon>();

	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		auto vertex = new nshade::Vertex();
		string line = fileLines.at(i);
		boost::split(parts, line, boost::is_any_of(" "));

		if (parts.empty()) // if line is blank
		{
			continue;
		}

		if (parts[0] == "v")
		{
			XMFLOAT3 position;
			auto x = boost::lexical_cast<float>(parts.at(1));
			auto y = boost::lexical_cast<float>(parts.at(2));
			auto z = boost::lexical_cast<float>(parts.at(3));

			if (isRightHand)
			{
				z = z * -1.0f;
				position = XMFLOAT3(x, y, z);
			}
			else
			{
				position = XMFLOAT3(x, y, z);
			}

			vertex->Position = position;
			vertex->Color = XMFLOAT3(1.0f, 1.0f, 1.0f);
			vertices->push_back(*vertex);
		}
	}

	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		// UV coords
		auto vertex = new nshade::Vertex();
		if (parts[0] == "vt")
		{
			auto x = boost::lexical_cast<float>(parts.at(1));
			auto y = boost::lexical_cast<float>(parts.at(2));

			if (isRightHand)
			{
				y = 1.0f - y;
			}

			XMFLOAT2 uv(x, y);
			vertex->UV = uv;
		}
	}
	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		// Normals
		auto vertex = new nshade::Vertex();
		if (parts[0] == "vn")
		{
			auto x = boost::lexical_cast<float>(parts.at(1));
			auto y = boost::lexical_cast<float>(parts.at(2));
			auto z = boost::lexical_cast<float>(parts.at(3));

			if (isRightHand)
			{
				z = z * -1.0f;
			}

			XMFLOAT3 normal(x, y, z);
			vertex->Normal = normal;
		}
	}
	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		// Polygons
		if (parts[0] == "f")
		{
			auto i1 = boost::lexical_cast<unsigned int>(parts.at(1));
			auto i2 = boost::lexical_cast<unsigned int>(parts.at(2));
			auto i3 = boost::lexical_cast<unsigned int>(parts.at(3));

			nshade::Polygon polygon;

			if (isRightHand)
			{
				polygon = { i3, i2, i1 };

				indices->push_back(i3);
				indices->push_back(i2);
				indices->push_back(i1);
			}
			else
			{
				polygon = { i1, i2, i3 };
				indices->push_back(i1);
				indices->push_back(i2);
				indices->push_back(i3);
			}

			polygons->push_back(polygon);
		}
	}

	return FillVertexAndIndexBuffer(vertices, indices);
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
		z = coordinate->mData[2] * scale * -1.0;
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
	static const nshade::Vertex cube[] =
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
	vertexBufferDesc.ByteWidth = sizeof(nshade::Vertex) * ARRAYSIZE(cube);
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