#pragma once

#include "common.h"
#include "light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(XMFLOAT3* color, FLOAT* intensity);
	~AmbientLight();
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
	shared_ptr<XMFLOAT3>	m_pColor;
	shared_ptr<FLOAT>		m_pIntensity;
};

