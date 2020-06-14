#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Light {
	XMFLOAT3 m_position;
	XMFLOAT3 m_color;
	XMFLOAT2 t;
};
