#pragma once
#include <vector>
#include <GL/glut.h>

template <typename T>
class Vec3
{
public:
	T x,y,z;
	Vec3() = default;
	Vec3( T x,T y, T z )
		:
		x( x ),
		y( y ),
		z( z )
	{}
	template <typename T3>
	explicit operator Vec3<T3>() const
	{
		return{ (T3)x,(T3)y,(T3)z };
	}
	Vec3 Normalize(Vec3& p)
	{
		T mag = p.x * p.x + p.y * p.y + p.z * p.z;
		mag = pow(mag, 0.5);
		p = p.operator/(mag);
		return p;
	}
	/*Vec4	operator-() const
	{
		return Vec4( -x,-y );
	}*/
	Vec3 cross(const Vec3& rhs) const
	{
		return Vec3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);
	}
	T dot(const Vec3& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	/*Vec3& max()
	{
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if (z < 0)
			z = 0;
		return *this;
	}*/
	Vec3& operator=( const Vec3 &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	
	Vec3& operator+=( const Vec3 &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vec3& operator-=( const Vec3 &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vec3 operator+( const Vec3 &rhs ) const
	{
		return Vec3( *this ) += rhs;
	}
	Vec3 operator-( const Vec3 &rhs ) const
	{
		return Vec3( *this ) -= rhs;
	}
	Vec3& operator*=( const T &rhs )
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vec3 operator*( const T &rhs ) const			//multiply const with vector
	{
		return Vec3( *this ) *= rhs;
	}
	Vec3& operator*=(const Vec3& rhs)		//for light color
	{
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}
	}
	Vec3 operator*(const Vec3& rhs) const
	{
		return Vec3(*this) *= rhs;
	}
	Vec3& operator/=( const T &rhs )
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	Vec3	operator/( const T &rhs ) const
	{
		return Vec3( *this ) /= rhs;
	}
	//bool	operator==( const Vec4 &rhs ) const
	//{
	//	return x == rhs.x && y == rhs.y;
	//}
	//bool	operator!=( const Vec4 &rhs ) const
	//{
	//	return !(*this == rhs);
	//}
};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Ved3d;
typedef Vec3<int> Vec3i;