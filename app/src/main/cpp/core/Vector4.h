#pragma once

#include <cmath>
#include <math.h>

namespace core
{

	struct Vector4
	{
		union
		{
			float v[2];

			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};


		inline Vector4(float _s = 0.0f) :
			x(_s),
			y(_s),
			z(_s),
			w(_s)
		{};

		inline Vector4(float _x, float _y, float _z, float _w) :
			x(_x),
			y(_y),
			z(_z),
			w(_w)
		{};

		inline Vector4(const Vector4& _v) :
			x(_v.x),
			y(_v.y),
			z(_v.z),
			w(_v.w)
		{};


		inline Vector4& operator= (const Vector4& _v)
		{
			x = _v.x;
			y = _v.y;
			z = _v.z;
			w = _v.w;
			return *this;
		};

		inline Vector4& operator+= (const Vector4& _v)
		{
			x += _v.x;
			y += _v.y;
			z += _v.z;
			w += _v.w;
			return *this;
		};

		inline Vector4& operator-= (const Vector4& _v)
		{
			x -= _v.x;
			y -= _v.y;
			z -= _v.z;
			w -= _v.w;
			return *this;
		};

		inline Vector4& operator*= (float _s)
		{
			x *= _s;
			y *= _s;
			z *= _s;
			w *= _s;
			return *this;
		};

		inline Vector4& operator/= (float _s)
		{
			return *this *= 1.0f / _s;
		};




		inline Vector4 operator+ (const Vector4& _v) const
		{
			return Vector4(*this) += _v;
		};

		inline Vector4 operator- (const Vector4& _v) const
		{
			return Vector4(*this) -= _v;
		};

		inline Vector4 operator* (float _s) const
		{
			return Vector4(*this) *= _s;
		};

		inline Vector4 operator/ (float _s) const
		{
			return Vector4(*this) /= _s;
		};


		inline float length() const
		{
			return std::sqrtf(x * x + y * y + z * z + w * w);
		};

		inline void normalise()
		{
			float lenSq = x * x + y * y + z * z + w * w;
			if (fabsf(lenSq - 1.0f) > EPSILON)
			{
				*this /= std::sqrtf(lenSq);
			}

		};



	};

}
