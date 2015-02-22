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
	vector<Light>			Lights;
	vector<Model>			Models;
	vector<ShaderSet>		Shaders;
	vector<ShaderSet>		GlobalShaders;
	vector<ShaderSet>		Materials;
	vector<nshade::Vertex>	Vertices;
	vector<unsigned int>	Indices;
};

