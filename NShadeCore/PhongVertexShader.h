#pragma once

#include "includes.h"
#include "shader.h"

namespace PhongShader
{
	struct InputLayout
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT4 AmbientColorIntensity;
		XMFLOAT4 LightPositionIntensity;
	};

	class PhongVertexShader : public Shader
	{
	public:
		PhongVertexShader(DeviceResources* pResources);
		~PhongVertexShader();
	};
}