#pragma once

#include "Export.h"
#include <DirectXMath.h>

using namespace DirectX;

class NSHADECORE_API Camera : public IUnknown
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

	virtual ULONG __stdcall Release() = 0;

private:
	XMMATRIX  viewMatrix;
	XMVECTOR  position;
	XMVECTOR  direction;
};
