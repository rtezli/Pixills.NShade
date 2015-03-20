#include "stdafx.h"
#include "model.h"

Model::Model(vector<NVertex> *vertices, vector<unsigned int> *indices)
{
    FillVertexAndIndexBuffer(vertices, indices);
}

HRESULT Model::FillVertexAndIndexBuffer(vector<NVertex>* modelVertices, vector<unsigned int>* modelIndexes)
{
    _vertexCount = modelVertices->size();
    _indexCount = modelIndexes->size();

    NVertex* vertexArr;
    vertexArr = (NVertex*)malloc(_vertexCount * sizeof(NVertex));
    copy(modelVertices->begin(), modelVertices->end(), vertexArr);

    D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
    vertexBufferDesc.ByteWidth = sizeof(NVertex) * _vertexCount;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
    vertexBufferData.pSysMem = vertexArr;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;


    auto result = Res::Get()->Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_vertexBuffer);
    if (FAILED(result))
    {
        return result;
    }

    unsigned int* indexArr;
    indexArr = (unsigned int*)malloc(_indexCount * sizeof(unsigned int));
    copy(modelIndexes->begin(), modelIndexes->end(), indexArr);

    D3D11_BUFFER_DESC indexBufferDesc = { 0 };
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * _indexCount;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
    indexBufferData.pSysMem = indexArr;
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    return Res::Get()->Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &_indexBuffer);
}

Model* Model::CreateCube(float size, XMFLOAT3* position)
{
    auto indices = new vector<unsigned int>();
    auto vertices = new vector<NVertex>();

    float halfSize = size / 2;
    for (auto i = 0; i < 2; i++)
    {
        for (auto j = 0; j < 2; j++)
        {
            for (auto k = 0; k < 2; k++)
            {
                XMFLOAT3 pos{ halfSize + i, halfSize, halfSize + j };
                auto vertex = new NVertex{ pos };
                vertices->push_back(*vertex);
            }
        }
    }

    return new Model(vertices, indices);
}

Model* Model::CreateHorizontalPlane(float size, XMFLOAT3* position)
{
    auto vertices = new vector<NVertex>();
    auto indices = new vector<unsigned int>();

    static const NVertex plane[] =
    {
        {
            XMFLOAT3(size * -1.0f + position->x, position->y, size * -1.0f + position->z),
            XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT2(0.0f, 100.0f),
            XMFLOAT3(0.0f, 0.0f, 0.0f)
        },
        {
            XMFLOAT3(size + position->x, position->y, size * -1.0f + position->z),
            XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT2(100.0f, 100.0f),
            XMFLOAT3(0.0f, 0.0f, 0.0f)
        },
        {
            XMFLOAT3(size + position->x, position->y, size + position->z),
            XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT2(100.0f, 0.0f),
            XMFLOAT3(0.0f, 0.0f, 0.0f)
        },
        {
            XMFLOAT3(size * -1.0f + position->x, position->y, size + position->z),
            XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT2(0.0f, 0.0f),
            XMFLOAT3(0.0f, 0.0f, 0.0f)
        }
    };

    for (auto i = 0; i < 4; i++)
    {
        vertices->push_back(plane[i]);
    }

    static const unsigned int planeIndices[] =
    {
        2, 1, 0,
        0, 3, 2
    };

    for (auto i = 0; i < 6; i++)
    {
        indices->push_back(planeIndices[i]);
    }

    return new Model(vertices, indices);
}

Model* Model::LoadModelFromFBXFile(char* fileName)
{
    auto vertices = new vector<NVertex>();
    auto indices = new vector<unsigned int>();
    nshade::FbxReader::Read(fileName, vertices, indices);
    return new Model(vertices, indices);
}

Model* Model::LoadModelFromOBJFile(char* fileName)
{
    auto vertices = new vector<NVertex>();
    auto indices = new vector<unsigned int>();
    ObjParser::Parse(vertices, indices, fileName);
    return new Model(vertices, indices);
}

void Model::AssignMaterial(Material *material)
{
    _material = shared_ptr<Material>(material);
}