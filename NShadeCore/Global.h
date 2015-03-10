#pragma once

#include "common.h"

class G
{
public:
	static G& I()
	{
		static G _instance;
		return _instance;
	}
	~G() {}
	DeviceResources* Resources;
private:
	G() {}
	G(const G&);
	G & operator = (const G &);
	static G* _instance;
};

