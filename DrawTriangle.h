#pragma once

#include <cmath>
#include <GL/glut.h>
#include "Vec3.h"
#include "Color.h"
#include "Line.h"

void DrawFlatTopTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, Color color)
{
	// calulcate slopes in screen space
	float m0 = (v2.x - v0.x) / (v2.y - v0.y);
	float m1 = (v2.x - v1.x) / (v2.y - v1.y);
	
	// calculate start and end scanlines
	const int yStart = (int)ceil(v0.y - 0.5f);
	const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = yStart; y < yEnd; y++)
	{
		// caluclate start and end points (x-coords)
		// add 0.5 to y value because we're calculating based on pixel CENTERS
		const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

		// calculate start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

		for (int x = xStart; x < xEnd; x++)
		{
			putPixel(x, y, color);
		}
	}
}

void DrawFlatBottomTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, Color color)
{
	// calulcate slopes in screen space
	float m0 = (v1.x - v0.x) / (v1.y - v0.y);
	float m1 = (v2.x - v0.x) / (v2.y - v0.y);

	// calculate start and end scanlines
	const int yStart = (int)ceil(v0.y - 0.5f);
	const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = yStart; y < yEnd; y++)
	{
		// caluclate start and end points
		// add 0.5 to y value because we're calculating based on pixel CENTERS
		const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

		// calculate start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

		for (int x = xStart; x < xEnd; x++)
		{
			putPixel(x, y, color);
		}
	}
}

void ScanLineTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, Color color)
{
	// using pointers so we can swap (for sorting purposes)
	const Vec3f* pv0 = &v0;
	const Vec3f* pv1 = &v1;
	const Vec3f* pv2 = &v2;

	// sorting vertices by y
	if (pv1->y < pv0->y) std::swap(pv0, pv1);
	if (pv2->y < pv1->y) std::swap(pv1, pv2);
	if (pv1->y < pv0->y) std::swap(pv0, pv1);

	if (pv0->y == pv1->y) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->x < pv0->x) std::swap(pv0, pv1);
		DrawFlatTopTriangle(*pv0, *pv1, *pv2, color);
	}
	else if (pv1->y == pv2->y) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->x < pv1->x) std::swap(pv1, pv2);
		DrawFlatBottomTriangle(*pv0, *pv1, *pv2, color);
	}
	else // general triangle
	{
		// find splitting vertex
		const float alphaSplit =
			(pv1->y - pv0->y) /
			(pv2->y - pv0->y);
		const Vec3f vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

		if (pv1->x < vi.x) // major right
		{
			DrawFlatBottomTriangle(*pv0, *pv1, vi, color);
			DrawFlatTopTriangle(*pv1, vi, *pv2, color);
		}
		else // major left
		{
			DrawFlatBottomTriangle(*pv0, vi, *pv1, color);
			DrawFlatTopTriangle(vi, *pv1, *pv2, color);
		}
	}
}

