#pragma once
#pragma warning( disable : 4996 )

#include "includes.h"
#include "material.h"

class Model
{
public:
	Model(DeviceResources* pResources);
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName, bool isRightHand);
	HRESULT CreateVertexAndIndexBuffer(XMFLOAT3 vertices[]);
	HRESULT CreateCube(float size, XMFLOAT3* position);
	HRESULT CreateHorizontalPlane(float size, XMFLOAT3* position);
	HRESULT	Initialize();

	static nshade::Vertex Cube[];
	static nshade::Vertex Sphere[];
private:
	HRESULT						TraverseAndStoreFbxNode1(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem);
	HRESULT						TraverseAndStoreFbxNode2(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem);
	HRESULT						TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh);
	HRESULT						FillVertexAndIndexBuffer(vector<unsigned int>* modelIndexes, vector<nshade::Vertex>* modelVertices);
	XMFLOAT3					ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, float scale);
	DeviceResources*			DeviceResource(){ return m_pDeviceResources.get(); }
	HRESULT						InitializeVertexBuffer();
	HRESULT						InitializeIndexBuffer(int indeces[]);
	HRESULT						InitializeConstantBuffer();
	HRESULT						SetTopology();
private:
	unsigned short				m_indexCount = 0;
	D3D11_BUFFER_DESC			m_bufferDesc;
	D3D11_SUBRESOURCE_DATA		m_initData;
	shared_ptr<DeviceResources>	m_pDeviceResources;
	Material*					m_pMaterial;
	vector<unsigned int>*		m_Indices;
	vector<nshade::Vertex>*		m_Vertices;
	D3D11_PRIMITIVE_TOPOLOGY	m_Topology;
};

#pragma warning( restore : 4996 )