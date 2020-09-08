#include "Camera.h"

Camera::Camera(float aspectRatio, float fovDeg, float posX, float posY, float posZ) :
	m_CameraPos({ posX, posY, posZ }), m_LookAt({ 0, 0, 0 }) {
	XMVECTOR eyePos = DirectX::XMVectorSet(posX, posY, posZ, .0f);
	XMVECTOR lookAtPos = DirectX::XMVectorSet(0.f, .0f, 0.f, .0f);
	XMVECTOR upVector = DirectX::XMVectorSet(.0f, 1.0f, .0f, .0f);

	m_UniformBuffer = new ConstantBuffer(VS_CONSTANT_CAMERA_BUFFER_SLOT, sizeof(VS_CONSTANT_CAMERA_BUFFER));
	m_ViewMatrix = XMMatrixLookAtLH(eyePos, lookAtPos, upVector);
	m_ProjMatrix = XMMatrixPerspectiveFovLH(XM_PI / 180.f * fovDeg, aspectRatio, .1f, 100.f);

	UpdateCamera();
}

Camera::~Camera() {
	delete m_UniformBuffer;
}

void Camera::SetUniformBuffer(VS_CONSTANT_CAMERA_BUFFER* buffer) {
	m_UniformBuffer->Map(sizeof(VS_CONSTANT_CAMERA_BUFFER), buffer);
	m_UniformBuffer->BindVS();
	m_UniformBuffer->BindPS();
}

void Camera::SetPosition(float x, float y, float z) {
	m_CameraPos = { x, y, z };
}

void Camera::SetLookAt(XMFLOAT3 vector) {
	m_LookAt = vector;
}


void Camera::UpdateCamera() {
	VS_CONSTANT_CAMERA_BUFFER buffer{};
	SYSTEMTIME stime;
	GetSystemTime(&stime);
	float time = (stime.wSecond * 1000. + stime.wMilliseconds) / 1000.;

	//XMVECTOR eye = XMVectorSet(std::sin(time) * 2, 1, std::cos(time) * 2, .0f);
	//XMVECTOR eye = XMVectorSet(0, 0, 2., .0f);
	XMVECTOR eye = XMLoadFloat3(&m_CameraPos);
	XMVECTOR lookAtPos = XMLoadFloat3(&m_LookAt);
	XMVECTOR upVector = DirectX::XMVectorSet(.0f, 1.0f, .0f, .0f);

	m_ViewMatrix = XMMatrixLookAtLH(eye, lookAtPos, upVector);

	XMStoreFloat4x4(&buffer.projMatrix, m_ProjMatrix);
	XMStoreFloat4x4(&buffer.viewMatrix, m_ViewMatrix);

	buffer.cameraPos = eye;
	SetUniformBuffer(&buffer);
}

XMFLOAT3 Camera::getCameraPosition() {
	return m_CameraPos;
}

XMFLOAT3 Camera::getLookAtPosition() {
	return m_LookAt;
}

