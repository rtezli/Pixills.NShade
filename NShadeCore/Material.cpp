#include "stdafx.h"

#include "material.h"


Material::Material()
{
	Texture = shared_ptr<ID3D11Texture2D>();
	BumpMap = shared_ptr<ID3D11Texture2D>();
	DisplacementMap = shared_ptr<ID3D11Texture2D>();
	Shaders = shared_ptr<ShaderSet>();
}

Material::~Material()
{
}
