#pragma once

#include "common.h"

class PreviewControl
{
public:
	PreviewControl(DeviceResources* pResources);
	~PreviewControl();

	HRESULT Initialize();
	HRESULT Shutdown();
	HRESULT Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();

private:
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 texture;
	};
	HRESULT InitializeBuffers(ID3D11Device*);
	HRESULT UpdateBuffers(ID3D11DeviceContext*, int, int);
	HRESULT RenderBuffers(ID3D11DeviceContext*);

private:
	DeviceResources*		m_pDeviceResources;
	ID3D11Buffer*			m_vertexBuffer;
	ID3D11Buffer*			m_indexBuffer;
	int						m_vertexCount, m_indexCount;
	int						m_screenWidth,m_screenHeight;
	int						m_bitmapWidth, m_bitmapHeight;
	int						m_previousPosX, m_previousPosY;
};

