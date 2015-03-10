#pragma once

#include "common.h"
#include "light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(DeviceResources* deviceResources, XMFLOAT3 *color, FLOAT *intensity);
protected:
	shared_ptr<XMFLOAT3>	_color;
	shared_ptr<FLOAT>		_intensity;
};

