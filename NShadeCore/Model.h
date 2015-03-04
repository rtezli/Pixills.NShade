#pragma once
#pragma warning( disable : 4996 )

#include "common.h"
#include "material.h"

class Model
{
public:
	Model(DeviceResources* pResources);
	~Model();
	HRESULT LoadModelFromFBXFile(char* fileName);
	HRESULT LoadModelFromOBJFile(char* fileName, bool isRightHand);
	HRESULT AssignMaterial(Material* pMaterial);
	HRESULT	Initialize();
	HRESULT	Render();

	vector<unsigned int>*		const GetIndices(){ return m_pIndices.get(); }
	vector<nshade::Vertex>*		const GetVertices(){ return m_pVertices.get(); }

	ID3D11Buffer*				const GetIndexBuffer(){ return m_pIndexBuffer.get(); }
	unsigned int				const GetIndexBufferSize(){ return GetIndices()->size(); }

	ID3D11Buffer*				const GetVertexBuffer(){ return m_pVertexBuffer.get(); }
	unsigned int				const GetVertexBufferSize(){ return GetVertices()->size(); }

	Material*					const GetMaterial(){ return m_pMaterial.get(); }
private:
	unsigned short						m_indexCount = 0;
	D3D11_BUFFER_DESC					m_bufferDesc;
	D3D11_SUBRESOURCE_DATA				m_initData;
	D3D11_PRIMITIVE_TOPOLOGY			m_Topology;

	DeviceResources*					m_pDeviceResources;
	shared_ptr<Material>				m_pMaterial;
	shared_ptr<vector<unsigned int>>	m_pIndices;
	shared_ptr<vector<nshade::Vertex>>	m_pVertices;
	shared_ptr<ID3D11Buffer>			m_pVertexBuffer;
	shared_ptr<ID3D11Buffer>			m_pIndexBuffer;
};

#pragma warning( restore : 4996 )