#pragma once

#include "includes.h"

using namespace std;
using namespace DirectX;

class DeviceResources
{

public:
	DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DeviceResources();
public:
	ID3D11Device*				Device;
	ID3D11DeviceContext*		DeviceContext;

	ID3D11Buffer*				VertexBuffer;
	ID3D11Buffer*				IndexBuffer;
	ID3D11Buffer*				ConstantBuffer;
	MVPConstantBuffer*			ConstantBufferData;

	ShaderSet*					Shaders;

	int							SamplesCount;
	int							IndexCount;
	int							ScreenWidth;
	int							ScreenHeight;
//private:
	//ID3D11Device*			const Device()						{ return m_pDevice.get(); }
	//ID3D11DeviceContext*	const DeviceContext()				{ return m_pDeviceContext.get(); }
	//XMFLOAT4X4*			const GetOrientationTransform3D()	{ return m_orientationTransform3D.get(); }

	//ID3D11Buffer*			const VertexBuffer()				{ return m_pVertexBuffer.get(); }
	//ID3D11Buffer*			const IndexBuffer()					{ return m_pIndexBuffer.get(); }
	//ID3D11Buffer*			const ConstantBuffer()				{ return m_pConstantBuffer.get(); }
	//MVPConstantBuffer*	const ConstantBufferData()			{ return m_pConstantBufferData.get(); }

	//shared_ptr<ID3D11Device>			m_pDevice;
	//shared_ptr<ID3D11DeviceContext>	m_pDeviceContext;
	//shared_ptr<ID3D11Buffer>			m_pIndexBuffer;
	//shared_ptr<ID3D11Buffer>			m_pVertexBuffer;
	//shared_ptr<ID3D11Buffer>			m_pConstantBuffer;
	//shared_ptr<MVPConstantBuffer>		m_pConstantBufferData;
	//shared_ptr<XMFLOAT4X4>			m_orientationTransform3D;
};

