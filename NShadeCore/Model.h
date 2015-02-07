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
	HRESULT	Initialize(std::vector<VertexPositionColor>* vertexes, unsigned int size);
 
	static const std::vector<VertexPositionColor> Cube;
	static const std::vector<VertexPositionColor> Sphere;
private:
	DeviceResources*			DeviceResource(){ return m_pDeviceResources.get(); }
	HRESULT						InitializeVertexBuffer(std::vector<VertexPositionColor>* vertexes);
	HRESULT						InitializeIndexBuffer(std::vector<int>* indexes);
	HRESULT						InitializeConstantBuffer();
	FbxScene*					ImportFbx(char* fileName);
private:
	unsigned short				m_indexCount = 0;
	D3D11_BUFFER_DESC			m_bufferDesc;
	D3D11_SUBRESOURCE_DATA		m_initData;
	shared_ptr<DeviceResources>	m_pDeviceResources;
};

