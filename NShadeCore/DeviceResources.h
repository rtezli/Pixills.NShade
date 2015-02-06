#pragma once

#include "includes.h"

using namespace DirectX;

class DeviceResources
{
public:
	DeviceResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DeviceResources();
public:
	ID3D11Device*			Device;
	ID3D11DeviceContext*	DeviceContext;
	XMFLOAT4X4*				GetOrientationTransform3D();
};

