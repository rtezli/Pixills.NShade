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
public:
	XMFLOAT4*	Color;
	XMFLOAT4*	Position;
};

