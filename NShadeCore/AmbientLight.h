#pragma once

#include "common.h"
#include "light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(XMFLOAT3 *pColor, FLOAT *pIntensity);
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 World;
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
		XMFLOAT4   LightPosition;
	};
	XMFLOAT4* GetColorIntensity();
private:
	shared_ptr<XMFLOAT3>	m_pColor;
	shared_ptr<FLOAT>		m_pIntensity;
};

