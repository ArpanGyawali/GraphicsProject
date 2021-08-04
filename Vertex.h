#pragma once

#include "Vec3.h"
#include "Color.h"

class Vertex
{
public:
	Vec3f pos;
	Vec3f normal;
	Vec3f color;
public:
	Vertex() = default;
	/*Vertex(const Vec3f& pos)
		:
		pos(pos)
	{}
	Vertex(const Vec3f& pos, const Vertex& src)
		:
		color(src.color),
		pos(pos)
	{}*/
	Vertex(const Vec3f& pos, const Vec3f& normal, const Vec3f& color)
		:
		color(color),
		normal(normal),
		pos(pos)
	{}
	Vertex& operator+=(const Vertex& rhs)
	{
		pos += rhs.pos;
		color += rhs.color;
		return *this;
	}
	Vertex operator+(const Vertex& rhs) const
	{
		return Vertex(*this) += rhs;
	}
	Vertex& operator-=(const Vertex& rhs)
	{
		pos -= rhs.pos;
		color -= rhs.color;
		return *this;
	}
	Vertex operator-(const Vertex& rhs) const
	{
		return Vertex(*this) -= rhs;
	}
	Vertex& operator*=(float rhs)
	{
		pos *= rhs;
		color *= rhs;
		return *this;
	}
	Vertex operator*(float rhs) const
	{
		return Vertex(*this) *= rhs;
	}
	Vertex& operator/=(float rhs)
	{
		pos /= rhs;
		color /= rhs;
		return *this;
	}
	Vertex operator/(float rhs) const
	{
		return Vertex(*this) /= rhs;
	}
};

//class Vertex
//{
//public:
//	Vec3f pos;
//	Vec3f tc;
//public:
//	Vertex() = default;
//	Vertex(const Vec3f& pos)
//		:
//		pos(pos)
//	{}
//	Vertex(const Vec3f& pos, const Vertex& src)
//		:
//		tc(src.tc),
//		pos(pos)
//	{}
//	Vertex(const Vec3f& pos, const Vec3f& tc)
//		:
//		tc(tc),
//		pos(pos)
//	{}
//	Vertex& operator+=(const Vertex& rhs)
//	{
//		pos += rhs.pos;
//		tc += rhs.tc;
//		return *this;
//	}
//	Vertex operator+(const Vertex& rhs) const
//	{
//		return Vertex(*this) += rhs;
//	}
//	Vertex& operator-=(const Vertex& rhs)
//	{
//		pos -= rhs.pos;
//		tc -= rhs.tc;
//		return *this;
//	}
//	Vertex operator-(const Vertex& rhs) const
//	{
//		return Vertex(*this) -= rhs;
//	}
//	Vertex& operator*=(float rhs)
//	{
//		pos *= rhs;
//		tc *= rhs;
//		return *this;
//	}
//	Vertex operator*(float rhs) const
//	{
//		return Vertex(*this) *= rhs;
//	}
//	Vertex& operator/=(float rhs)
//	{
//		pos /= rhs;
//		tc /= rhs;
//		return *this;
//	}
//	Vertex operator/(float rhs) const
//	{
//		return Vertex(*this) /= rhs;
//	}
//};