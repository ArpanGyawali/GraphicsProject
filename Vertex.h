#pragma once

#include "Vec3.h"
#include "Color.h"

struct Vertex
{
	Vec3f position;
	//Vec3f texcoord;
	Vec3f normal;
	bool backFace;
};