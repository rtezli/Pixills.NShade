#pragma once

#include "Export.h"
#include "d3d11.h"
#include "directxmath.h"
#include "fbxsdk.h"

using namespace DirectX;

namespace NShade{
	class NSHADECORE_API Model
	{
	public:
		Model();
		~Model();
		HRESULT LoadModelFromFBXFile(CHAR* fileName);
		HRESULT LoadModelFromOBJFile(CHAR* fileName);
	private:
		FbxScene* NShade::Model::FbxImport(CHAR* fileName);
		void FillVerticesFromFbxImport(FbxScene* scene);
	private:
		HRESULT CreateBuffer();
		ID3D11Device* m_pDevice;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
		D3D11_BUFFER_DESC m_bufferDesc;
		D3D11_SUBRESOURCE_DATA m_initData;
	};
}
