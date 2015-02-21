#pragma once

#include "includes.h"
#include "model.h"

class Scene
{
public:
	Scene();
	~Scene();
	HRESULT Render();
public:
	XMFLOAT3* AmbientColor;
	nshade::Light* Lights;
	Model* Models;
};

