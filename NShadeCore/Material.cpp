#include "stdafx.h"
#include "Material.h"

void Material::AssignShaders(Shaders *shaders)
{
    _shaders = shared_ptr<Shaders>(shaders);
}