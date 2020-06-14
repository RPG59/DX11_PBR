#pragma once

#include<DirectXMath.h>

using namespace DirectX;

struct Material {
	XMFLOAT3 albedo = { .5, 0., 0. };
	float mettalic = .5f;
	float roughness = .1f;
	float ao = 1.f;
	float t1;
	float t2;
};
