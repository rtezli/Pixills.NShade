#pragma once

#include "includes.h"
#include "fbxsdk.h"

class Model
{
private:
	struct NSVERTEX2
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

	struct NSVERTEX3
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;
		DirectX::XMFLOAT3 Uv;
	};
public:
	Model();
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName);
	HRESULT	Initialize(ID3D11Device* pDevice, std::vector<NSVERTEX2>* vertexes, unsigned int size);
 
	static const std::vector<Model::NSVERTEX2> Cube;
	static const std::vector<Model::NSVERTEX2> Sphere;
private:
	HRESULT						InitializeVertexBuffer(std::vector<NSVERTEX2>* vertexes);
	HRESULT						InitializeIndexBuffer(std::vector<NSVERTEX2>* indexes);
	FbxScene*					ImportFbx(char* fileName);
private:
	std::shared_ptr<ID3D11Device> m_pDevice;
	std::shared_ptr<ID3D11Buffer> m_pVertexBuffer;
	std::shared_ptr<ID3D11Buffer> m_pIndexBuffer;

	unsigned short			m_indexCount = 0;
	D3D11_BUFFER_DESC		m_bufferDesc;
	D3D11_SUBRESOURCE_DATA	m_initData;
};