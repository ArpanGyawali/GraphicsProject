#pragma once

#include <vector>
#include "Vec3.h"

struct VertexIndexBuf
{
	VertexIndexBuf(std::vector<Vec3f> verts, std::vector<size_t> ind)
		:
		vertices(std::move(verts)),
		indices(std::move(ind))
	{
		backFace.resize(indices.size() / 3);
	}
	std::vector<Vec3f> vertices;
	std::vector<size_t> indices;
	std::vector<bool> backFace;
};
//#pragma once
//
//#include <vector>
//#include "Vec3.h"
//
//struct VertexIndexBuf
//{
//	VertexIndexBuf(std::vector<Vec3f> verts)
//		:
//		vertices(std::move(verts))
//		//indices(std::move(ind))
//	{
//		backFace.resize(vertices.size() / 3, false);
//	}
//	std::vector<Vec3f> vertices;
//	//std::vector<size_t> indices;
//	std::vector<bool> backFace;
//};

//#pragma once
//
//#include <vector>
//#include "Vec3.h"
//
//template<class T>
//class VertexIndexBuf
//{
//	VertexIndexBuf(std::vector<Vec3f> verts, std::vector<size_t> ind)
//		:
//		vertices(std::move(verts)),
//		indices(std::move(ind))
//	{
//		assert(vertices.size() > 2);
//		assert(indices.size() % 3 == 0);
//	}
//	std::vector<vec3f> vertices;
//	std::vector<size_t> indices;
//};