#include "Sphere.h"

Sphere::Sphere() {


	UINT32 indexCount;

	const UINT32 X_SEGMENTS = 64;
	const UINT32 Y_SEGMENTS = 64;

	for (UINT32 y = 0; y <= Y_SEGMENTS; ++y) {
		for (UINT32 x = 0; x <= X_SEGMENTS; ++x) {
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.f * XM_PI) * std::sin(ySegment * XM_PI);
			float yPos = std::cos(ySegment * XM_PI);
			float zPos = std::sin(xSegment * 2.f * XM_PI) * std::sin(ySegment * XM_PI);

			data.push_back({ xPos, yPos, zPos, xPos, yPos, zPos, xSegment, ySegment });
		}
	}

	bool oddRow = false;
	for (UINT32 y = 0; y < Y_SEGMENTS; ++y) {
		if (!oddRow) {
			for (UINT32 x = 0; x <= X_SEGMENTS; ++x) {
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else {
			for (INT32 x = X_SEGMENTS; x >= 0; --x) {
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}

		oddRow = !oddRow;
	}
}
