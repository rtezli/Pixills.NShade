#pragma once

#include "includes.h"

class TilePool
{
public:
	TilePool(DeviceResources* pResources);
	~TilePool();
public:
	HRESULT Initialize();
private:
	DeviceResources* m_pResources;
};

