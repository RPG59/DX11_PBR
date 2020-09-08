#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

#define VS_CONSTANT_CAMERA_BUFFER_SLOT 0

struct VS_CONSTANT_CAMERA_BUFFER {
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;
	XMVECTOR cameraPos;
};

class Camera
{
private:
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjMatrix;
	ConstantBuffer* m_UniformBuffer;
	XMFLOAT3 m_CameraPos;
	XMFLOAT3 m_LookAt;
public:
	Camera(float aspectRatio, float fovDeg, float posX, float posY, float posZ);
	~Camera();
	void UpdateCamera();
	void SetUniformBuffer(VS_CONSTANT_CAMERA_BUFFER*);
	void SetPosition(float, float, float);
	void SetLookAt(XMFLOAT3);
	XMFLOAT3 getCameraPosition();
	XMFLOAT3 getLookAtPosition();

};

