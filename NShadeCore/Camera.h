#pragma once

#include "includes.h"
#include "deviceresources.h"

using namespace DirectX;

class Camera
{
public:
	Camera(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	~Camera();
public:
	void Camera::Initialize(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	void RotateHorizontal(float Angle);
	void RotateVertical(float Angle);
	void MoveX(float Angle);
	void MoveY(float Angle);
	void MoveZ(float Angle);
private:
	struct MATRIX_BUFFER*						m_constBuffer;
	struct DEVICE_RESOURCES*					m_pDeviceResources;
	std::shared_ptr<DirectX::XMFLOAT4X4>		m_position;
	std::shared_ptr<DirectX::XMFLOAT4X4>		m_direction;
	DirectX::XMFLOAT4X4*						m_constantBufferData;
	std::shared_ptr<DeviceResources>			m_deviceResources;
};

struct MATRIX_BUFFER
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};