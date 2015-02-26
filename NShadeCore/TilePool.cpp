#include "stdafx.h"

#include "tilepool.h"

TilePool::TilePool(DeviceResources* pResources)
{
	m_pResources = pResources;
}

TilePool::~TilePool()
{
}

HRESULT TilePool::Initialize()
{
	return 0;
}