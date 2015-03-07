#include "stdafx.h"

#include "previewcontrol.h"

PreviewControl::PreviewControl(DeviceResources* resources)
{
	_deviceResources = resources;
}

HRESULT PreviewControl::Initialize()
{
	return 0;
}

HRESULT PreviewControl::Render(ID3D11DeviceContext* deviceContext, INT positionX, INT positionY)
{
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	auto result = UpdateBuffers(deviceContext, positionX, positionY);
	if (FAILED(result))
	{
		return result;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return 0;
}

HRESULT PreviewControl::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	int i;


	_vertexCount = 6;
	_indexCount = _vertexCount;

	vertices = new VertexType[_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[_indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * _vertexCount));

	for (i = 0; i< _indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	auto result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return 0;
}

HRESULT PreviewControl::RenderBuffers(ID3D11DeviceContext* context)
{
	return 0;
}

HRESULT PreviewControl::UpdateBuffers(ID3D11DeviceContext* context, INT positionX, INT positionY)
{
	return 0;
}