#pragma once

#include "Export.h"
#include "d3d11.h"
#include "directxmath.h"
#include "fbxsdk.h"
#include "vector"

using namespace DirectX;



class NSHADECORE_API Model : public IUnknown
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
	HRESULT Initialize(ID3D11Device* pDevice, NSVERTEX2* pModel[]);
	virtual ULONG __stdcall Release() = 0;
	static const NSVERTEX2 Model::Cube[8];
	static const NSVERTEX2 Model::Sphere[8];

private:
	HRESULT		Model::InitializeVertexBuffer(NSVERTEX2* vertices[]);
	HRESULT		Model::InitializeIndexBuffer(NSVERTEX2* indexes[]);
	FbxScene*	Model::FbxImport(CHAR* fileName);
	void FillVerticesFromFbxImport(FbxScene* scene);
private:
	USHORT m_indexCount;
	ID3D11Device* m_pDevice;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	D3D11_BUFFER_DESC m_bufferDesc;
	D3D11_SUBRESOURCE_DATA m_initData;
};