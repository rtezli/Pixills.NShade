#pragma once

#include "Export.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace NShade{
	class NSHADECORE_API Camera
	{
	public:
		Camera();
		~Camera();

		void Build(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		void RotateHorizontal(float Angle);
		void RotateVertical(float Angle);
		void MoveX(float Angle);
		void MoveY(float Angle);
		void MoveZ(float Angle);
	private:
		XMMATRIX  viewMatrix;
		XMVECTOR  position;
		XMVECTOR  direction;
	};
}