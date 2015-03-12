#pragma once

#include "common.h"

class FpsTextRenderer
{
public:
	FpsTextRenderer(DeviceResources *resources);
	~FpsTextRenderer();
	HRESULT Initialize();
private:
	ID3D11Device*			const	Device(){ return Resources()->Device; }
	ID3D11DeviceContext*	const	DeviceContext(){ return Resources()->DeviceContext; }
	DeviceResources*		const	Resources(){ return _deviceResources; }
	DeviceResources*				_deviceResources;
};

