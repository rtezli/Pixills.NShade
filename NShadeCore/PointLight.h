#pragma once

#include "common.h"
#include "light.h"

class PointLight : public Light
{
public:
	PointLight(XMFLOAT3* position, XMFLOAT3* color, FLOAT* intensity);
	~PointLight();
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
private:
	shared_ptr<XMFLOAT3>	m_pPosition;
	shared_ptr<XMFLOAT3>	m_pColor;
	shared_ptr<FLOAT>		m_pIntensity;
};

