#pragma once

#include "Vec3.h"
#include <vector>
#include "VertexIndexBuf.h"

class Cube
{
private:
	std::vector<Vec3f> vertices;
	std::vector<Vec2f> tc;
public:
	Cube(float size)
	{
		const float side = size / 2.0f;
		vertices.emplace_back(0.0f,0.0f,side); // 0				
		vertices.emplace_back(side, 0.0f, side); // 1
		vertices.emplace_back(side, side, side); // 2
		vertices.emplace_back(0, side, side); // 3
		vertices.emplace_back(0.0f, 0.0f, 0.0f); // 4
		vertices.emplace_back(0.0f, side, 0.0f); // 5
		vertices.emplace_back(side, side, 0.0f); // 6
		vertices.emplace_back(side, 0.0f, 0.0f); // 7
	}

	VertexIndexBuf<Vec3f> GetTriangles() const
	{
		/*return{				//for wire framed
			vertices,{
				0,1,  1,3,  3,2,  2,0,
				0,4,  1,5,  3,7,  2,6,
				4,5,  5,7,  7,6,  6,4 }
		};*/
		return{					//for rasterized solid
			vertices,{
			0,1,2, 2,3,0,			//front
			1,7,6, 6,2,1,			//right
			2,6,5, 5,3,2,			//top
			4,5,6, 6,7,4,			//back
			0,3,5, 5,4,0,			//left
			0,4,7, 7,1,0 }
		};
	}
};
	