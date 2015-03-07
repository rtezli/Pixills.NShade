#pragma once

#include "common.h"

class PreviewControl
{
public:
	PreviewControl(DeviceResources* pResources);
	~PreviewControl();

	HRESULT Initialize();
	HRESULT Shutdown();
	HRESULT Render(ID3D11DeviceContext*, INT, INT);

	int GetIndexCount();

private:
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 texture;
	};
	HRESULT InitializeBuffers(ID3D11Device*);
	HRESULT UpdateBuffers(ID3D11DeviceContext*, INT, INT);
	HRESULT RenderBuffers(ID3D11DeviceContext*);

private:
	DeviceResources*		_deviceResources;
	ID3D11Buffer*			_vertexBuffer;
	ID3D11Buffer*			_indexBuffer;
	INT						_vertexCount, _indexCount;
	INT						_screenWidth, _screenHeight;
	INT						_bitmapWidth, _bitmapHeight;
	INT						_previousPosX, _previousPosY;
};

