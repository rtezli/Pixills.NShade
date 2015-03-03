#pragma once
#pragma warning( disable : 4996 )

#include "includes.h"
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

	vector<unsigned int>*		const GetIndices(){ return m_Indices.get(); }
	vector<nshade::Vertex>*		const GetVertices(){ return m_Vertices.get(); }
	ID3D11Buffer*				const GetIndexBuffer(){ return m_IndexBuffer.get(); }
	ID3D11Buffer*				const GetVertexBuffer(){ return m_VertexBuffer.get(); }
	Material*					const GetMaterial(){ return m_pMaterial.get(); }
private:
	HRESULT						CreateVertexBuffer();
	HRESULT						CreateIndexBuffer();
	HRESULT						SetTopology(char verticesPerFace);
private:
	unsigned short						m_indexCount = 0;
	D3D11_BUFFER_DESC					m_bufferDesc;
	D3D11_SUBRESOURCE_DATA				m_initData;
	D3D11_PRIMITIVE_TOPOLOGY			m_Topology;

	DeviceResources*					m_pDeviceResources;
	shared_ptr<Material>				m_pMaterial;
	shared_ptr<vector<unsigned int>>	m_Indices;
	shared_ptr<vector<nshade::Vertex>>	m_Vertices;
	shared_ptr<ID3D11Buffer>			m_VertexBuffer;
	shared_ptr<ID3D11Buffer>			m_IndexBuffer;
};

#pragma warning( restore : 4996 )