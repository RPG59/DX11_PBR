#pragma once

#include <DirectXMath.h>
#include <vector>

#include "VertexBuffer.h"
#include "Util.h"

using namespace DirectX;

struct SphereData {
	Vec3 position;
	Vec3 normal;
	Vec2 UV;
};

class Sphere
{
public:
	std::vector<SphereData> data;
	std::vector<UINT32> indices;

	Sphere();
};

