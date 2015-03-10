#pragma once

#include "common.h"
#include "light.h"

class PointLight : public Light
{
public:
	PointLight(DeviceResources* deviceResources, XMFLOAT3 *position, XMFLOAT3 *color, FLOAT *intensity);

	shared_ptr<XMFLOAT3>			_position;
	shared_ptr<XMFLOAT3>			_color;
	shared_ptr<FLOAT>				_intensity;
};

