#pragma once

#include "Export.h"
#include "d3d11.h"
#include "directxmath.h"
#include "fbxsdk.h"
#include "vector"

using namespace DirectX;

class Model : public IUnknown
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
	HRESULT Model::Initialize(ID3D11Device* pDevice, std::vector<NSVERTEX2> vertexes);
	virtual ULONG __stdcall Release() = 0;
	static const std::vector<Model::NSVERTEX2> Cube;
	static const std::vector<Model::NSVERTEX2> Sphere;

private:
	HRESULT Model::InitializeVertexBuffer(std::vector<NSVERTEX2> vertexes);
	HRESULT Model::InitializeIndexBuffer(std::vector<NSVERTEX2> indexes);
	FbxScene*	Model::FbxImport(CHAR* fileName);
	void FillVerticesFromFbxImport(FbxScene* scene);
private:
	ID3D11Device* m_pDevice = 0;
	ID3D11Buffer* m_pVertexBuffer = 0;
	ID3D11Buffer* m_pIndexBuffer = 0;
	USHORT m_indexCount = 0;
	D3D11_BUFFER_DESC m_bufferDesc;
	D3D11_SUBRESOURCE_DATA m_initData;
};