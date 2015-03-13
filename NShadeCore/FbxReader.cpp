#include "stdafx.h"

#include "fbxreader.h"

HRESULT nshade::FbxReader::Read(CHAR* fileName, vector<NVertex>* vertices, vector<UINT>* indices)
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
    nshade::FbxReader::TraverseChildren(fbxRootNode, mesh);

    nshade::FbxReader::TraverseAndStoreFbxNode(mesh, &axisSystem, vertices, indices);
    return 0;
}

HRESULT nshade::FbxReader::TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh)
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

HRESULT nshade::FbxReader::TraverseAndStoreFbxNode(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem, vector<NVertex>* vertices, vector<UINT>* indices)
{
    auto count = nodes->size();

    // The scene maybe
    for (UINT i = 0; i < count; i++)
    {
        auto child = nodes->at(i);

        auto mesh = child->GetMesh();
        auto controlPoints = mesh->GetControlPoints();
        auto controlPointsCount = mesh->GetControlPointsCount();

        // Copy all control points
        for (auto cp = 0; cp < controlPointsCount; cp++)
        {
            auto point = controlPoints[cp];
            auto newVertex = new NVertex();
            // Set vertex position (and invert Z)
            newVertex->Position = FbxReader::ConvertFbxVector4ToXMFLOAT3(&point, axisSystem, 1.0f);
            vertices->push_back(*newVertex);
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
                auto newVertex = new NVertex(vertices->at(vertexIndex));

                // Create the normal
                FbxVector4 normal;
                mesh->GetPolygonVertexNormal(p, v, normal);
                newVertex->Normal = XMFLOAT3
                {
                    static_cast<FLOAT>(normal.mData[0]),
                    static_cast<FLOAT>(normal.mData[1]),
                    static_cast<FLOAT>(normal.mData[2])
                };

                switch (v)
                {
                case 0:
                {
                    newVertex->PolyPosition = XMFLOAT3{ 1.0f, 0.0f, 0.0f };
                    break;
                }
                case 1:
                {
                    newVertex->PolyPosition = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
                    break;
                }
                case 2:
                {
                    newVertex->PolyPosition = XMFLOAT3{ 0.0f, 0.0f, 1.0f };
                    break;
                }
                }

                // Use a standard color for all vertices
                newVertex->Color = XMFLOAT4
                {
                    0.7f, 0.2f, 0.7f, 0.6f
                };

                // Dont set that now
                newVertex->UV = XMFLOAT2
                {
                    0.0f, 0.0f
                };
                vertices->at(vertexIndex) = *newVertex;
                indices->push_back(vertexIndex);
            }
        }
    }
    //reverse(indices->begin(), indices->end());
    return 0;
}

XMFLOAT3 nshade::FbxReader::ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, FLOAT scale)
{
    auto rightHanded = true;
    auto coordSystem = axisSystem->GetCoorSystem();
    if (coordSystem != FbxAxisSystem::ECoordSystem::eRightHanded)
    {
        rightHanded = false;
    }

    auto yUp = true;
    auto xFront = false;

    auto upInverter = 1.0f;
    INT upVectorSign;
    auto upVector = axisSystem->GetUpVector(upVectorSign);
    if (upVectorSign != 1)
    {
        upInverter = -1.0f;
    }
    if (upVector != FbxAxisSystem::eYAxis)
    {
        yUp = false;
    }

    auto frontInverter = 1.0f;
    INT frontVectorSign;
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
        static_cast<FLOAT>(x),
        static_cast<FLOAT>(y),
        static_cast<FLOAT>(z)
    };
    return dxVector;
}
