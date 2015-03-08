#pragma once

#include "common.h"
#include "light.h"

class PointLight : public Light
{
public:
	PointLight(DeviceResources* deviceResources, XMFLOAT3 *position, XMFLOAT3 *color, FLOAT *intensity);
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4X4 refection;
		XMFLOAT3   camera;
		FLOAT	   time;
	};
	ID3D11Buffer*			const GetConstBuffer(){ return _constBuffer.get(); };
	ConstantBufferData*		const GetConstBufferData(){ return _constBufferData.get(); };
	XMFLOAT4*				GetPositionIntensity();
private:
	VOID							CreateResources();
	VOID							InitializeConstantBuffer();

	DeviceResources*				_deviceResources;
	shared_ptr<XMFLOAT3>			_position;
	shared_ptr<XMFLOAT3>			_color;
	shared_ptr<FLOAT>				_intensity;
	shared_ptr<ID3D11Buffer>		_constBuffer;
	shared_ptr<ConstantBufferData>	_constBufferData;
};

