#pragma once
#pragma warning( disable : 4996 )

#include "res.h"
#include "vector"
#include "memory"
#include "fbxreader.h"
#include "objparser.h"
#include "material.h"
#include "phongvertexshader.h"

using namespace std;

EXTERN class API Model
{
public:
    Model();
public:
    HRESULT                 LoadModelFromFBXFile(char *fileName);
    HRESULT                 LoadModelFromOBJFile(char *fileName, bool isRightHand);
    HRESULT                 CreateCube(float size, XMFLOAT3 *position);
    HRESULT                 CreateHorizontalPlane(float size, XMFLOAT3 *position);
    HRESULT                 Initialize();

    void                    AssignMaterial(Material *material);
    Material*               GetMaterial(){ return _material.get(); }

    vector<NVertex>*        GetVertices(){ return _vertices.get(); }
    unsigned int            GetVertexCount(){ return _vertexCount; }
    ID3D11Buffer*           GetVertexBuffer(){ return _vertexBuffer; }

    vector<unsigned int>*   GetIndices(){ return _indices.get(); }
    unsigned int            GetIndexCount(){ return _indexCount; }
    ID3D11Buffer*           GetIndexBuffer(){ return _indexBuffer; }

    static NVertex          Cube[];
    static NVertex          Sphere[];
private:
    HRESULT             TraverseAndStoreFbxNode1(vector<FbxNode*> *nodes, FbxAxisSystem *axisSystem);
    HRESULT             TraverseAndStoreFbxNode2(vector<FbxNode*> *nodes, FbxAxisSystem *axisSystem);
    HRESULT             TraverseChildren(FbxNode *node, vector<FbxNode*> *mesh);
    HRESULT             FillVertexAndIndexBuffer(vector<unsigned int> *modelIndexes, vector<NVertex> *modelVertices);
    XMFLOAT3            ConvertFbxVector4ToXMFLOAT3(FbxVector4 *coordinate, FbxAxisSystem *axisSystem, float scale);

    HRESULT             InitializeVertexBuffer();
    HRESULT             InitializeIndexBuffer(int indeces[]);
    HRESULT             SetTopology(char verticesPerFace);
private:
    shared_ptr<vector<NVertex>>         _vertices;
    shared_ptr<vector<unsigned int>>    _indices;
    shared_ptr<Material>                _material;
    D3D11_BUFFER_DESC                   _bufferDesc;
    D3D11_SUBRESOURCE_DATA              _initData;
    D3D11_PRIMITIVE_TOPOLOGY            _topology;

    ID3D11Buffer                        *_vertexBuffer;
    unsigned int                        _vertexCount;
    ID3D11Buffer                        *_indexBuffer;
    unsigned int                        _indexCount;
};

#pragma warning( restore : 4996 )