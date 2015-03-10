#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight(DeviceResources *deviceResources, XMFLOAT3 *position, XMFLOAT3 *color, FLOAT *intensity) : Light(deviceResources)
{	
	_position = shared_ptr<XMFLOAT3>(position);
	_color = shared_ptr<XMFLOAT3>(color);
	_intensity = shared_ptr<FLOAT>(intensity);
	_parameters = shared_ptr<XMFLOAT4>(new XMFLOAT4(position->x, position->y, position->z, *intensity));
	InitializeBuffer();
}