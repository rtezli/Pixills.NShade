#pragma once

#include "includes.h"
#include "fbxsdk.h"
#include "fbxsdk\scene\fbxaxissystem.h"

class Model
{
public:
	Model(DeviceResources* pResources);
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName);
	HRESULT CreateVertexAndIndexBuffer(XMFLOAT3 vertices[]);
	HRESULT	Initialize();

	static Vertex Cube[];
	static Vertex Sphere[];
private:
	HRESULT						TraverseAndStoreFbxNode1(FbxNode* fbxRootNode, FbxAxisSystem* axisSystem);
	HRESULT						TraverseAndStoreFbxNode2(FbxNode* fbxRootNode, FbxAxisSystem* axisSystem);
	HRESULT						FillVertexAndIndexBuffer(vector<Vertex>* modelVertices, vector<unsigned int>* modelIndexes);
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

