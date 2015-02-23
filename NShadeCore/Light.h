#pragma once

#include "includes.h"

enum LightType : char
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
	Light();
	~Light();
private :
	XMFLOAT3	m_Color;
	XMFLOAT3	m_Position;
	XMFLOAT3	m_Direction;
	float		m_Intensity;
	LightType	m_Type;
};

