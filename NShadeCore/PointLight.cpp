#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight(XMFLOAT3* position, XMFLOAT3* color, FLOAT* intensity)
{
	m_pPosition = shared_ptr<XMFLOAT3>(position);
	m_pColor = shared_ptr<XMFLOAT3>(color);
	m_pIntensity = shared_ptr<FLOAT>(intensity);
}


PointLight::~PointLight()
{
}
