#pragma once

#include "includes.h"

class DeviceResources
{
public:
	DeviceResources();
	~DeviceResources();
public:
	DirectX::XMFLOAT4X4* GetOrientationTransform3D();
};

