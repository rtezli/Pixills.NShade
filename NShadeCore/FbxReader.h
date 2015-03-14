#pragma once

#include "vector"
#include "res.h"
#include "d3d11.h"
#include "directxmath.h"
#include "fbxsdk.h"
#include "fbxsdk/scene/fbxaxissystem.h"

using namespace std;

namespace nshade
{
    class FbxReader
    {
    public:
        static HRESULT Read(char *fileName, vector<NVertex> *vertices, vector<unsigned int> *indices);
    private:
        static HRESULT  TraverseChildren(FbxNode *node, vector<FbxNode*> *mesh);
        static HRESULT  TraverseAndStoreFbxNode(vector<FbxNode*> *nodes, FbxAxisSystem *axisSystem, vector<NVertex> *vertices, vector<unsigned int> *indices);
        static XMFLOAT3 ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, float scale);
    };
}