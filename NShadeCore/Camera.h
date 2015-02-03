#pragma once

#include "includes.h"

class Camera
{
public:
	Camera();
	~Camera();
public:
	void Build(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	void RotateHorizontal(float Angle);
	void RotateVertical(float Angle);
	void MoveX(float Angle);
	void MoveY(float Angle);
	void MoveZ(float Angle);
private:
	std::shared_ptr<DirectX::XMFLOAT4X4>	m_viewMatrix;
	std::shared_ptr<DirectX::XMVECTOR>		m_position;
	std::shared_ptr<DirectX::XMVECTOR>		m_direction;
};
