#include "stdafx.h"
#include "AmbientLight.h"


AmbientLight::AmbientLight(XMFLOAT3* color, FLOAT* intensity)
{
	m_pColor = shared_ptr<XMFLOAT3>(color);
	m_pIntensity = shared_ptr<FLOAT>(intensity);
}


AmbientLight::~AmbientLight()
{
}

