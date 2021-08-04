#pragma once

#define PI 3.1428
#include "Vec3.h"

template <typename T>
class Mat4
{
public:
	T elements[4][4];
	Mat4& operator=(const Mat4& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	/*Mat4& operator*=(T rhs)
	{
		for (auto& row : elements)
		{
			for (T& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}*/

	Mat4 operator*(T rhs) const
	{
		Mat4 result = *this;
		return result = result * rhs;
	}

	Mat4 operator*(const Mat4& rhs) const
	{
		Mat4 result;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				T sum = (T)0.0;
				for (size_t k = 0; k < 4; k++)
				{
					sum += elements[i][k] * rhs.elements[k][j];
				}
				result.elements[i][j] = sum;
			}
		}
		return result;
	}

	constexpr static Mat4 Identity()
	{
		return {
			(T)1.0,(T)0.0,(T)0.0,(T)0.0,
			(T)0.0,(T)1.0,(T)0.0,(T)0.0,
			(T)0.0,(T)0.0,(T)1.0,(T)0.0,
			(T)0.0,(T)0.0,(T)0.0,(T)1.0,
		};
	}

	constexpr static Mat4 Scaling(T factor)
	{
		return {
			factor,(T)0.0,(T)0.0,(T)0.0,
			(T)0.0,factor,(T)0.0,(T)0.0,
			(T)0.0,(T)0.0,factor,(T)0.0,
			(T)0.0,(T)0.0,(T)0.0,(T)1.0,
		};
	}

	static Mat4 RotationZ(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);	
		return {
			cosTheta, -sinTheta, (T)0.0,(T)0.0,
			sinTheta, cosTheta, (T)0.0,(T)0.0,
			(T)0.0,    (T)0.0,   (T)1.0,(T)0.0,
			(T)0.0,	   (T)0.0,   (T)0.0,(T)1.0,
		};
	}

	static Mat4 RotationY(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return {
			cosTheta, (T)0.0, sinTheta,(T)0.0,
			(T)0.0,   (T)1.0, (T)0.0,   (T)0.0,
			-sinTheta, (T)0.0, cosTheta, (T)0.0,
			(T)0.0,   (T)0.0, (T)0.0,   (T)1.0,
		};
	}

	static Mat4 RotationX(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return {
			(T)1.0, (T)0.0,   (T)0.0,  (T)0.0,
			(T)0.0, cosTheta, -sinTheta,(T)0.0,
			(T)0.0, sinTheta, cosTheta,(T)0.0,
			(T)0.0, (T)0.0,   (T)0.0,  (T)1.0,
		};
	}

	/*template<class V>
	constexpr static Mat4 Translation(const V& tl)
	{
		return Translation(tl.x, tl.y, tl.z);
	}*/

	constexpr static Mat4 Translation(T tx, T ty, T tz)
	{
		return {
			(T)1.0,(T)0.0,(T)0.0,(T)tx,
			(T)0.0,(T)1.0,(T)0.0,(T)ty,
			(T)0.0,(T)0.0,(T)1.0,(T)tz,
			(T)0.0,(T)0.0,(T)0.0,(T)1.0,
		};
	}

	constexpr static Mat4 RotView(Vec3f right, Vec3f up, Vec3f dir)
	{
		return {
			right.x,right.y,right.z,0.0f,
			up.x,up.y,up.z,0.0f,
			dir.x,dir.y,dir.z,0.0f,
			0.0f,0.0f,0.0f,1.0f,
		};
	}

	constexpr static Mat4 TranslateView(Vec3f pos)
	{
		return {
			1.0f,0.0f,0.0f,-pos.x,
			0.0f,1.0f,0.0f,-pos.y,
			0.0f,0.0f,1.0f,-pos.z,
			0.0f,0.0f,0.0f,1.0f,
		};
	}

	constexpr static Mat4 PerspectiveFOV(T fov, T ar, T n, T f)
	{
		const auto fov_rad = fov * (T)PI / (T)180.0;
		const auto w = (T)1.0f / std::tan(fov_rad / (T)2.0);
		const auto h = w * ar;
		return {
			w,		(T)0.0,	(T)0.0,				(T)0.0,
			(T)0.0,	h,		(T)0.0,				(T)0.0,
			(T)0.0,	(T)0.0,	f / (f - n),		(T)1.0,
			(T)0.0,	(T)0.0,	-n * f / (f - n),	(T)0.0,
		};
	}

	Vec3f operator*(const Vec3f& lhs)
	{
		return{
			lhs.x * elements[0][0] + lhs.y * elements[0][1] + lhs.z * elements[0][2] + elements[0][3],
			lhs.x * elements[1][0] + lhs.y * elements[1][1] + lhs.z * elements[1][2] + elements[1][3],
			lhs.x * elements[2][0] + lhs.y * elements[2][1] + lhs.z * elements[2][2] + elements[2][3]
		};
	}
};

//template<typename T>
//_Vec3<T>& operator*=(_Vec3<T>& lhs, const Mat4<T, 3>& rhs)
//{
//	return lhs = lhs * rhs;
//}

//template<typename T>
//Vec3<T> operator*(const _Vec3<T>& lhs, const Mat4<T, 3>& rhs)
//{
//	return{
//		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
//		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
//		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
//	};
//}

//template<typename T>
//_Vec4<T>& operator*=(_Vec4<T>& lhs, const Mat4<T, 4>& rhs)
//{
//	return lhs = lhs * rhs;
//}


typedef Mat4<float> Mat4f;
typedef Mat4<double> Mat4d;
typedef Mat4<int> Mat4i;