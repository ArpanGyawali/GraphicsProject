#pragma once

template <typename T>
class Vec2
{
public:
	T x;
	T y;
	Vec2() = default;
	Vec2(T x, T y)
		:
		x( x ),
		y( y )
	{}
	template <typename T2>
	explicit operator Vec2<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	Vec2	operator-() const
	{
		return Vec2(-x, -y);
	}
	Vec2& operator=(const Vec2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	Vec2& operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T operator*(const Vec2& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	Vec2	operator+(const Vec2& rhs) const
	{
		return Vec2(*this) += rhs;
	}
	Vec2	operator-(const Vec2& rhs) const
	{
		return Vec2(*this) -= rhs;
	}
	Vec2& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vec2	operator*(const T& rhs) const
	{
		return Vec2(*this) *= rhs;
	}
	Vec2& operator/=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vec2	operator/(const T& rhs) const
	{
		return Vec2(*this) /= rhs;
	}
};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;

