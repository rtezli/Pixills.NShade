#pragma once

#include "export.h"
#include "d3d11.h"
#include "directxmath.h"
#include "fbxsdk.h"
#include "memory"
#include "vector"

using namespace DirectX;

class Model
{
private:
	struct NSVERTEX2
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};

	struct NSVERTEX3
	{
		XMFLOAT3 Position;
		XMFLOAT3 Color;
		XMFLOAT3 Uv;
	};
public:
	Model();
	~Model();
	HRESULT LoadModelFromFBXFile(CHAR* fileName);
	HRESULT LoadModelFromOBJFile(CHAR* fileName);
	HRESULT Model::Initialize(ID3D11Device* pDevice, std::vector<NSVERTEX2>* vertexes, UINT size);
	//virtual ULONG __stdcall Release() = 0;
	static const std::vector<Model::NSVERTEX2> Cube;
	static const std::vector<Model::NSVERTEX2> Sphere;

private:
	HRESULT Model::InitializeVertexBuffer(std::vector<NSVERTEX2>* vertexes);
	HRESULT Model::InitializeIndexBuffer(std::vector<NSVERTEX2>* indexes);
	FbxScene* Model::FbxImport(CHAR* fileName);
	void FillVerticesFromFbxImport(FbxScene* scene);
private:
	std::shared_ptr<ID3D11Device> m_pDevice;
	std::shared_ptr<ID3D11Buffer> m_pVertexBuffer;
	std::shared_ptr<ID3D11Buffer> m_pIndexBuffer;

	USHORT m_indexCount = 0;
	D3D11_BUFFER_DESC m_bufferDesc;
	D3D11_SUBRESOURCE_DATA m_initData;
};