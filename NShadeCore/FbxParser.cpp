#include "stdafx.h"

#include "fbxparser.h"


FbxParser::FbxParser()
{
}

FbxParser::~FbxParser()
{
}

HRESULT FbxParser::Read(char* fileName, vector<nshade::Vertex>* vertices, vector<unsigned int>* indices)
{
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
	FbxParser::TraverseChildren(fbxRootNode, mesh);

	FbxParser::TraverseAndStoreFbxNode2(mesh, &axisSystem, vertices, indices);
	return 0;
}

HRESULT FbxParser::TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh)
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

HRESULT FbxParser::TraverseAndStoreFbxNode1(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem, vector<nshade::Vertex>* vertices, vector<unsigned int>* indices)
{
	auto count = nodes->size();

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
			newVertex->Position = FbxParser::ConvertFbxVector4ToXMFLOAT3(&point, axisSystem, 1.0f);
			vertices->push_back(*newVertex);
		}

		auto polygonCount = mesh->GetPolygonCount();

		//For each polygon in the model
		for (auto p = 0; p < polygonCount; p++)
		{
			auto polygonSize = mesh->GetPolygonSize(p);
			//For each point in a polygon get :  cooradinates, normals and index
			for (auto v = 0; v < polygonSize; v++)
			//for (auto v = polygonSize - 1; v >= 0; v--)
			{
				auto vertexIndex = mesh->GetPolygonVertex(p, v);
				auto point = controlPoints[vertexIndex];
				auto newVertex = vertices->at(vertexIndex);

				// Create the normal
				FbxVector4 normal;
				mesh->GetPolygonVertexNormal(p, v, normal);
				newVertex.Normal = XMFLOAT3
				{
					static_cast<float>(normal.mData[0]),
					static_cast<float>(normal.mData[1]),
					static_cast<float>(normal.mData[2])
				};

				// Use a standard color for all vertices
				newVertex.Color = XMFLOAT3
				{
					0.9f, 0.7f, 1.0f
				};

				// Dont set that now
				newVertex.UV = XMFLOAT2
				{
					0.0f, 0.0f
				};

				indices->push_back(vertexIndex);
			}
		}
	}
	//reverse(indices->begin(), indices->end());
	return 0;
}

HRESULT FbxParser::TraverseAndStoreFbxNode2(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem, vector<nshade::Vertex>* vertices, vector<unsigned int>* indices)
{
	auto count = nodes->size();

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

			newVertex->Position = FbxParser::ConvertFbxVector4ToXMFLOAT3(&point, axisSystem, 1.0);
			newVertex->Color = XMFLOAT3{ 0.9f, 0.7f, 1.0f };
			newVertex->UV = XMFLOAT2{ 0.0f, 0.0f };
			newVertex->Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };

			vertices->push_back(*newVertex);
		}

		for (auto v = 0; v < vertexCount; v++)
		{
			indices->push_back(static_cast<unsigned int>(vertexIndexes[v]));
		}
	}
	return 0;
}

XMFLOAT3 FbxParser::ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, float scale)
{
	bool rightHanded = true;
	auto coordSystem = axisSystem->GetCoorSystem();
	if (coordSystem != FbxAxisSystem::ECoordSystem::eRightHanded)
	{
		rightHanded = false;
	}

	bool yUp = true;
	bool xFront = false;

	int upInverter = 1.0f;
	int upVectorSign;
	auto upVector = axisSystem->GetUpVector(upVectorSign);
	if (upVectorSign != 1)
	{
		upInverter = -1.0f;
	}
	if (upVector != FbxAxisSystem::eYAxis)
	{
		yUp = false;
	}

	int frontInverter = 1.0f;
	int frontVectorSign;
	auto frontVector = axisSystem->GetFrontVector(frontVectorSign);
	if (frontVectorSign != -1)
	{
		frontInverter = -1.0f;
	}

	auto x = 0.0f;
	auto y = 0.0f;
	auto z = 0.0f;

	XMFLOAT3 dxVector;

	if (xFront)
	{
		x = coordinate->mData[2];
		y = coordinate->mData[1];// *upInverter;
		z = coordinate->mData[0];// *frontInverter;
	}
	else
	{
		x = coordinate->mData[0];
		y = coordinate->mData[1];// * upInverter;
		z = coordinate->mData[2];// * frontInverter;
	}

	dxVector = XMFLOAT3
	{
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(z)
	};
	return dxVector;
}