#pragma once

#include <cmath>
#include <math.h>

#define EPSILON 0.00001f

namespace core
{

	struct Vector3
	{
		union
		{
			float v[3];

			struct
			{
				float x;
				float y;
				float z;
			};
		};


		inline Vector3(float _s = 0.0f) :
			x(_s),
			y(_s),
			z(_s)
		{};

		inline Vector3(float _x, float _y, float _z) :
			x(_x),
			y(_y),
			z(_z)
		{};

		inline Vector3(const Vector3& _v) :
			x(_v.x),
			y(_v.y),
			z(_v.z)
		{};


		inline Vector3& operator= (const Vector3& _v)
		{
			x = _v.x;
			y = _v.y;
			z = _v.z;
			return *this;
		};

		inline Vector3& operator+= (const Vector3& _v)
		{
			x += _v.x;
			y += _v.y;
			z += _v.z;
			return *this;
		};

		inline Vector3& operator-= (const Vector3& _v)
		{
			x -= _v.x;
			y -= _v.y;
			z -= _v.z;
			return *this;
		};

		inline Vector3& operator*= (float _s)
		{
			x *= _s;
			y *= _s;
			z *= _s;
			return *this;
		};

		inline Vector3& operator/= (float _s)
		{
			return *this *= 1.0f / _s;
		};




		inline Vector3 operator+ (const Vector3& _v) const
		{
			return Vector3(*this) += _v;
		};

		inline Vector3 operator- (const Vector3& _v) const
		{
			return Vector3(*this) -= _v;
		};

		inline Vector3 operator* (float _s) const
		{
			return Vector3(*this) *= _s;
		};

		inline Vector3 operator/ (float _s) const
		{
			return Vector3(*this) /= _s;
		};


		inline float length() const
		{
			return std::sqrtf(x * x + y * y + z * z);
		};

		inline void normalize()
		{
			float lenSq = x * x + y * y + z * z;
			if (fabsf(lenSq - 1.0f) > EPSILON)
			{
				*this /= std::sqrtf(lenSq);
			}

		};



	};

}
