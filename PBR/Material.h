#pragma once

#include<DirectXMath.h>

using namespace DirectX;

struct Material {
	XMFLOAT4 albedo = { .5, 0., 0., 0. };
	XMFLOAT4 m_r_ao = { 1., .5, .1, 0 }; // metallic, roughness, ao
};

//struct Material {
//	XMVECTOR ambient{ 1., .5, .31, 0 };
//	XMVECTOR dffuse{ 1., .5, .31, 0 };
//	XMVECTOR specular{ .5, .5, .5, 0 };
//	XMVECTOR shininess{ 32., 0, 0, 0 };
//};
