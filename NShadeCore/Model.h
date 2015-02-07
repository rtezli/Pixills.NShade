#pragma once

#include "includes.h"
#include "fbxsdk.h"

class Model
{
public:
	Model(DeviceResources* pResources);
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName);
	HRESULT	Initialize(vector<VertexPositionColor>* vertexes, unsigned int size);
 
	static const vector<VertexPositionColor> Cube;
	static const vector<VertexPositionColor> Sphere;
private:
	DeviceResources*			DeviceResource(){ return m_pDeviceResources.get(); }
	HRESULT						InitializeVertexBuffer(vector<VertexPositionColor>* vertexes);
	HRESULT						InitializeIndexBuffer(vector<int>* indexes);
	HRESULT						InitializeConstantBuffer();
	FbxScene*					ImportFbx(char* fileName);
private:
	unsigned short				m_indexCount = 0;
	D3D11_BUFFER_DESC			m_bufferDesc;
	D3D11_SUBRESOURCE_DATA		m_initData;
	shared_ptr<DeviceResources>	m_pDeviceResources;
};

