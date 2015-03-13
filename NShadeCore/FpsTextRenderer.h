#pragma once

#include "common.h"

EXTERN class API FpsTextRenderer
{
public:
	FpsTextRenderer(DeviceResources *resources);
	HRESULT Initialize();
private:
	DeviceResources			*_resources;
};

