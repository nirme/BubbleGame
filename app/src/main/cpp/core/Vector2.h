#pragma once

#include <cmath>
#include <math.h>

#define EPSILON 0.00001f

namespace core
{

	struct Vector2
	{
		union
		{
			float v[2];

			struct
			{
				float x;
				float y;
			};
		};


		inline Vector2(float _s = 0.0f) :
			x(_s),
			y(_s)
		{};

		inline Vector2(float _x, float _y) :
			x(_x), 
			y(_y)
		{};

		inline Vector2(const Vector2& _v) :
			x(_v.x), 
			y(_v.y)
		{};


		inline Vector2& operator= (const Vector2& _v)
		{
			x = _v.x;
			y = _v.y;
			return *this;
		};

		inline Vector2& operator+= (const Vector2& _v)
		{
			x += _v.x;
			y += _v.y;
			return *this;
		};

		inline Vector2& operator-= (const Vector2& _v)
		{
			x -= _v.x;
			y -= _v.y;
			return *this;
		};

        inline Vector2& operator+= (float _s)
        {
            x += _s;
            y += _s;
            return *this;
        };

        inline Vector2& operator-= (float _s)
        {
            return *this += -_s;
        };

        inline Vector2& operator*= (float _s)
        {
            x *= _s;
            y *= _s;
            return *this;
        };

        inline Vector2& operator/= (float _s)
        {
            return *this *= 1.0f / _s;
        };




        inline Vector2 operator+ (const Vector2& _v) const
		{
			return Vector2(*this) += _v;
		};

		inline Vector2 operator- (const Vector2& _v) const
		{
			return Vector2(*this) -= _v;
		};

		inline Vector2 operator+ (float _s) const
		{
			return Vector2(*this) += _s;
		};

		inline Vector2 operator- (float _s) const
		{
			return Vector2(*this) -= _s;
		};

		inline Vector2 operator* (float _s) const
		{
			return Vector2(*this) *= _s;
		};

		inline Vector2 operator/ (float _s) const
		{
			return Vector2(*this) /= _s;
		};

		inline Vector2 operator- () const
		{
			return Vector2(-this->x, -this->y);
		};


		inline float length() const
		{
			return std::sqrt(x * x + y * y);
		};

		inline void normalize()
		{
			float lenSq = x * x + y * y;
			if (std::fabs(lenSq - 1.0f) > EPSILON)
			{
				*this /= std::sqrt(lenSq);
			}

		};


		inline bool operator== (const Vector2& _v) const
		{
			return (std::fabs(x - _v.x) <= EPSILON && std::fabs(y - _v.y) <= EPSILON);
		};

		inline bool operator!= (const Vector2& _v) const
		{
			return !(*this == _v);
		};

	};

}
