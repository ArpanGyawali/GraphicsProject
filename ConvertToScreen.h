#pragma once

#include <GL/glut.h>
#include "Vec3.h"

class ConvertToScreen
{										//normalize -1 1            //screen resolution
private:
	float xFactor;
	float yFactor;
public:
	ConvertToScreen()
		:
		xFactor(float(GLUT_WINDOW_WIDTH )/2.0f),
		yFactor(float(GLUT_WINDOW_HEIGHT) /2.0f)
	{}
	Vec3f& Transform(Vec3f& normalized) const
	{
		//const float zInv = 1.0f / normalized.z;
		normalized.x = normalized.x * xFactor;
		normalized.y = normalized.y * yFactor;
		return normalized;
	}
	Vec3f GetScreenCoordinate(Vec3f& normalized) const
	{
		return Transform( normalized );
	}
};
