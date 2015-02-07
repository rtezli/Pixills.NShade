#pragma once

#include "includes.h"
#include "fbxsdk.h"

class Model
{
public:
	Model();
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName);
	HRESULT	Initialize(ID3D11Device* pDevice, std::vector<VertexPositionColor>* vertexes, unsigned int size);
 
	static const std::vector<VertexPositionColor> Cube;
	static const std::vector<VertexPositionColor> Sphere;
private:
	HRESULT						InitializeVertexBuffer(std::vector<VertexPositionColor>* vertexes);
	HRESULT						InitializeIndexBuffer(std::vector<int>* indexes);
	FbxScene*					ImportFbx(char* fileName);
private:
	std::shared_ptr<ID3D11Device> m_pDevice;
	std::shared_ptr<ID3D11Buffer> m_pVertexBuffer;
	std::shared_ptr<ID3D11Buffer> m_pIndexBuffer;

	unsigned short			m_indexCount = 0;
	D3D11_BUFFER_DESC		m_bufferDesc;
	D3D11_SUBRESOURCE_DATA	m_initData;
};

