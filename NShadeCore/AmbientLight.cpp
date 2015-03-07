#include "stdafx.h"
#include "AmbientLight.h"


AmbientLight::AmbientLight(XMFLOAT3 *pColor, FLOAT *pIntensity)
{
	m_pColor = shared_ptr<XMFLOAT3>(pColor);
	m_pIntensity = shared_ptr<FLOAT>(pIntensity);
}

XMFLOAT4* AmbientLight::GetColorIntensity()
{
	return new XMFLOAT4(m_pColor->x, m_pColor->y, m_pColor->x, m_pIntensity);
}
