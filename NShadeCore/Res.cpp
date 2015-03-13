#include "stdafx.h"
#include "res.h"

Res* Res::instance = 0;
Res* Res::Get()
{
	if (!instance)
	{
		instance = new Res();
	}
	return instance;
}