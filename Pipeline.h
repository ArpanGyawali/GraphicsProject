#pragma once

#include "Triangle.h"
#include "VertexIndexBuf.h"
#include "ConvertToScreen.h"
#include "DrawTriangle.h"
#include "Mat4.h"
#include <algorithm>
#include "Line.h"

// fixed-function triangle drawing pipeline
// draws textured triangle lists with clamping
template<typename T>
inline T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

class Pipeline
{
private:
	ConvertToScreen screen;
	/*std::unique_ptr<Surface> pTex;*/
public:
	// vertex type used for geometry and throughout pipeline
	class Vertex
	{
	public:
		Vec3f pos;
		Vec2f t;
	public:
		Vertex() = default;
		Vertex(const Vec3f& pos)
			:
			pos(pos)
		{}
		// this enables template functions clone a vertex
		// while changing the pos only
		Vertex(const Vec3f& pos, const Vertex& src)
			:
			t(src.t),
			pos(pos)
		{}
		Vertex(const Vec3f& pos, const Vec3f& t)
			:
			t(t),
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	};
public:
	Pipeline() = default;
	void Draw(VertexIndexBuf<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}
	void BindRotation(const Mat4f& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Mat4f& translation_in)
	{
		Mat4f translation = translation_in;
	}
	/*void BindTexture(const std::wstring& filename)
	{
		pTex = std::make_unique<Surface>(Surface::FromFile(filename));
	}*/
private:
	Mat4f rotation;
	Mat4f translation;
	// vertex processing function
	// transforms vertices and then passes vtx & idx lists to triangle assembler
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// create vertex vector for vs output
		std::vector<Vertex> verticesOut;

		// transform vertices using matrix + vector
		for (const auto& v : vertices)
		{

			verticesOut.emplace_back(rotation * translation * v.pos, v.t);
		}

		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
	}
	// triangle assembly function
	// assembles indexed vertex stream into triangles and passes them to post process
	// culls (does not send) back facing triangles
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// assemble triangles in the stream and process
		for (size_t i = 0, end = indices.size() / 3;
			i < end; i++)
		{
			// determine triangle vertices via indexing
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			Vec3f normal = (v2.pos - v1.pos).cross(v0.pos - v1.pos);
			if (Vec3f(0.0f, 0.0f, 1.0f).dot(normal) >= 0.0f)
			{
				// process 3 vertices into a triangle
				ProcessTriangle(v0, v1, v2);
			}
		}
	}
	// triangle processing function
	// takes 3 vertices to generate triangle
	// sends generated triangle to post-processing
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		// generate triangle from 3 vertices using gs
		// and send to post-processing
		Triangle<Vertex> triangle = { v0, v1, v2 };
		PostProcessTriangleVertices(triangle);
	}
	// vertex post-processing function
	// perform perspective and viewport transformations
	void PostProcessTriangleVertices(Triangle<Vertex>& triangle)
	{
		// perspective divide and screen transform for all 3 vertices
		screen.Transform(triangle.v0.pos);
		screen.Transform(triangle.v1.pos);
		screen.Transform(triangle.v2.pos);

		// draw the triangle
		DrawTriangle(triangle);
	}
	// === triangle rasterization functions ===
	//   it0, it1, etc. stand for interpolants
	//   (values which are interpolated across a triangle in screen space)
	//
	// entry point for tri rasterization
	// sorts vertices, determines case, splits to flat tris, dispatches to flat tri funcs
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		// using pointers so we can swap (for sorting purposes)
		const Vertex* pv0 = &triangle.v0;
		const Vertex* pv1 = &triangle.v1;
		const Vertex* pv2 = &triangle.v2;

		ScanLineTriangle(pv0->pos, pv1->pos, pv2->pos, WHITE);

	//	// sorting vertices by y
	//	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
	//	if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
	//	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

	//	if (pv0->pos.y == pv1->pos.y) // natural flat top
	//	{
	//		// sorting top vertices by x
	//		if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);

	//		DrawFlatTopTriangle(*pv0, *pv1, *pv2);
	//	}
	//	else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
	//	{
	//		// sorting bottom vertices by x
	//		if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);

	//		DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
	//	}
	//	else // general triangle
	//	{
	//		// find splitting vertex interpolant
	//		const float alphaSplit =
	//			(pv1->pos.y - pv0->pos.y) /
	//			(pv2->pos.y - pv0->pos.y);
	//		const auto vi = interpolate(*pv0, *pv2, alphaSplit);

	//		if (pv1->pos.x < vi.pos.x) // major right
	//		{
	//			DrawFlatBottomTriangle(*pv0, *pv1, vi);
	//			DrawFlatTopTriangle(*pv1, vi, *pv2);
	//		}
	//		else // major left
	//		{
	//			DrawFlatBottomTriangle(*pv0, vi, *pv1);
	//			DrawFlatTopTriangle(vi, *pv1, *pv2);
	//		}
	//	}
	//}
	//// does flat *TOP* tri-specific calculations and calls DrawFlatTriangle
	//void DrawFlatTopTriangle(const Vertex& it0,
	//	const Vertex& it1,
	//	const Vertex& it2)
	//{
	//	// calulcate dVertex / dy
	//	// change in interpolant for every 1 change in y
	//	const float delta_y = it2.pos.y - it0.pos.y;
	//	const auto dit0 = (it2 - it0) / delta_y;
	//	const auto dit1 = (it2 - it1) / delta_y;

	//	// create right edge interpolant
	//	auto itEdge1 = it1;

	//	// call the flat triangle render routine
	//	DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	//}
	//// does flat *BOTTOM* tri-specific calculations and calls DrawFlatTriangle
	//void DrawFlatBottomTriangle(const Vertex& it0,
	//	const Vertex& it1,
	//	const Vertex& it2)
	//{
	//	// calulcate dVertex / dy
	//	// change in interpolant for every 1 change in y
	//	const float delta_y = it2.pos.y - it0.pos.y;
	//	const auto dit0 = (it1 - it0) / delta_y;
	//	const auto dit1 = (it2 - it0) / delta_y;

	//	// create right edge interpolant
	//	auto itEdge1 = it0;

	//	// call the flat triangle render routine
	//	DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	//}
	//// does processing common to both flat top and flat bottom tris
	//// texture lookup and pixel written here
	//void DrawFlatTriangle(const Vertex& it0,
	//	const Vertex& it1,
	//	const Vertex& it2,
	//	const Vertex& dv0,
	//	const Vertex& dv1,
	//	Vertex itEdge1)
	//{
	//	// create edge interpolant for left edge (always v0)
	//	auto itEdge0 = it0;

	//	// calculate start and end scanlines
	//	const int yStart = (int)ceil(it0.pos.y - 0.5f);
	//	const int yEnd = (int)ceil(it2.pos.y - 0.5f); // the scanline AFTER the last line drawn

	//	// do interpolant prestep
	//	itEdge0 += dv0 * (float(yStart) + 0.5f - it0.pos.y);
	//	itEdge1 += dv1 * (float(yStart) + 0.5f - it0.pos.y);

	//	// prepare clamping constants
	//	const float tex_width = float(pTex->GetWidth());
	//	const float tex_height = float(pTex->GetHeight());
	//	const float tex_xclamp = tex_width - 1.0f;
	//	const float tex_yclamp = tex_height - 1.0f;

	//	for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
	//	{
	//		// calculate start and end pixels
	//		const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
	//		const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f); // the pixel AFTER the last pixel drawn

	//		// create scanline interpolant startpoint
	//		// (some waste for interpolating x,y,z, but makes life easier not having
	//		//  to split them off, and z will be needed in the future anyways...)
	//		auto iLine = itEdge0;

	//		// calculate delta scanline interpolant / dx
	//		const float dx = itEdge1.pos.x - itEdge0.pos.x;
	//		const auto diLine = (itEdge1 - iLine) / dx;

	//		// prestep scanline interpolant
	//		iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);

	//		for (int x = xStart; x < xEnd; x++, iLine += diLine)
	//		{
	//			// perform texture lookup, clamp, and write pixel
	//			putPixel(x, y, pTex->GetPixel(
	//				(unsigned int)std::min(iLine.t.x * tex_width + 0.5f, tex_xclamp),
	//				(unsigned int)std::min(iLine.t.y * tex_height + 0.5f, tex_yclamp)
	//			));
	//		}
	//	}
	}
};
