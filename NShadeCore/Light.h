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

class Light : public ConstantBufferProvider
{
public:
	Light();
	ID3D11ShaderResourceView*	const GetResourceView(){ return _resourceView.get(); };
public:
	shared_ptr<XMFLOAT4>					_color;
	shared_ptr<XMFLOAT4>					_position;
	shared_ptr<ID3D11ShaderResourceView>	_resourceView;
};

