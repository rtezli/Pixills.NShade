#pragma once
#pragma warning( disable : 4996 )

#include "res.h"
#include "vector"
#include "memory"
#include "fbxreader.h"
#include "objparser.h"
#include "material.h"

using namespace std;

EXTERN class API Model
{
public:
    Model();
public:
    void                    AssignMaterial(Material *material);
    Material*               GetMaterial(){ return _material.get(); }

    vector<NVertex>*        GetVertices(){ return _vertices.get(); }
    unsigned int            GetVertexCount(){ return _vertexCount; }
    ID3D11Buffer*           GetVertexBuffer(){ return _vertexBuffer; }

    vector<unsigned int>*   GetIndices(){ return _indices.get(); }
    unsigned int            GetIndexCount(){ return _indexCount; }
    ID3D11Buffer*           GetIndexBuffer(){ return _indexBuffer; }
    
    static Model*           LoadModelFromFBXFile(char *fileName);
    static Model*           LoadModelFromOBJFile(char *fileName);

    static Model*           CreateCube(float size, XMFLOAT3 *position);
    static Model*           CreateHorizontalPlane(float size, XMFLOAT3 *position);
private:
    Model(vector<NVertex>   *vertices, vector<unsigned int> *indices);
    HRESULT                 FillVertexAndIndexBuffer(vector<NVertex>* modelVertices, vector<unsigned int>* modelIndexes);
private:
    shared_ptr<vector<NVertex>>         _vertices;
    shared_ptr<vector<unsigned int>>    _indices;
    shared_ptr<Material>                _material;

    ID3D11Buffer                        *_vertexBuffer;
    unsigned int                        _vertexCount;
    ID3D11Buffer                        *_indexBuffer;
    unsigned int                        _indexCount;
};

#pragma warning(default : 4996 )