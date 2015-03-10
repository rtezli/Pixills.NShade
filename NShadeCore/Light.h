#pragma once

#include "common.h"

enum LightType : CHAR
{
	Directional = 1,
	Point = 2,
	Spot = 3,
	Volume = 4,
	Area = 5,
	Ambient = 6
};

class Light
{
public:
	Light(DeviceResources *deviceResources);
public:
	XMFLOAT4*		const Parameters(){ return _parameters.get(); };
	ID3D11Buffer*	const GetBuffer(){ return _buffer.get(); };
protected:
	VOID						InitializeBuffer();
	shared_ptr<ID3D11Buffer>	_buffer;
	DeviceResources*			_deviceResources;
	shared_ptr<XMFLOAT4>		_parameters;
};

