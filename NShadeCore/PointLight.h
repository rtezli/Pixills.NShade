#pragma once

#include "common.h"
#include "light.h"

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();
public:
	struct ConstantBufferData
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4X4 refection;
		XMFLOAT3   camera;
		float	   time;
	};
};

