#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Light {
	XMFLOAT4 m_position;
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_specular;
};
