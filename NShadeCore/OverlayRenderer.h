#pragma once

#include "common.h"

class FpsTextRenderer
{
public:
	FpsTextRenderer(DeviceResources *pResources);
	~FpsTextRenderer();
	HRESULT Initialize();
private:
	ID3D11Device*			const	Device(){ return Resources()->Device; }
	ID3D11DeviceContext*	const	DeviceContext(){ return Resources()->DeviceContext; }
	DeviceResources*		const	Resources(){ return m_pDeviceResources; }
	DeviceResources*				m_pDeviceResources;
};

