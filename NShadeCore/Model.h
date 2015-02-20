#pragma once
#pragma warning( disable : 4996 ) 

#include "includes.h"
#include "fbxsdk.h"
#include "fbxsdk/scene/fbxaxissystem.h"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

class Model
{
public:
	Model(DeviceResources* pResources);
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName);
	HRESULT CreateVertexAndIndexBuffer(XMFLOAT3 vertices[]);
	HRESULT	Initialize();

	static nshade::Vertex Cube[];
	static nshade::Vertex Sphere[];
private:
	HRESULT						TraverseAndStoreFbxNode1(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem);
	HRESULT						TraverseAndStoreFbxNode2(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem);
	HRESULT						TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh);
	HRESULT						FillVertexAndIndexBuffer(vector<nshade::Vertex>* modelVertices, vector<unsigned int>* modelIndexes);
	XMFLOAT3					ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, float scale);
	DeviceResources*			DeviceResource(){ return m_pDeviceResources.get(); }
	HRESULT						InitializeVertexBuffer();
	HRESULT						InitializeIndexBuffer(int indeces[]);
	HRESULT						InitializeConstantBuffer();
private:
	unsigned short				m_indexCount = 0;
	D3D11_BUFFER_DESC			m_bufferDesc;
	D3D11_SUBRESOURCE_DATA		m_initData;
	shared_ptr<DeviceResources>	m_pDeviceResources;
};

#pragma warning( restore : 4996 ) 