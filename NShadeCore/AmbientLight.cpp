#include "stdafx.h"
#include "AmbientLight.h"


AmbientLight::AmbientLight(DeviceResources *deviceResources, XMFLOAT3 *color, FLOAT *intensity) : Light(deviceResources)
{
	_color = shared_ptr<XMFLOAT3>(color);
	_intensity = shared_ptr<FLOAT>(intensity);
	_parameters = shared_ptr<XMFLOAT4>(new XMFLOAT4(color->x, color->y, color->z, *intensity));
	InitializeBuffer();
}