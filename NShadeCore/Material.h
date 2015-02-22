#pragma once

#include "includes.h"
#include "shader.h"

class Material
{
public:
	Material();
	~Material();

public:
	XMFLOAT4			Color;
	ID3D10Texture2D*	Texture;
	ID3D10Texture2D*	BumpMap;
	ID3D10Texture2D*	DisplaceMentMap;
	vector<Shader>		Shaders;
	float				SpecularPower;
};

