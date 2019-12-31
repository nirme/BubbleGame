#pragma once

#include <cmath>
#include "../Vector2.h"



namespace core
{
	namespace _2d
	{

		class Quaternion
		{
		public:
			union
			{
				float _q[2];
				struct
				{
					float w;
					float z;
				};
			};

			inline Quaternion() :
				w(1.0f), 
				z(0.0f)
			{};

			inline Quaternion(float _t) :
				w(std::cos(_t * 0.5f)), 
				z(-std::sin(_t * 0.5f))
			{};

			inline Quaternion(const Quaternion& _q) :
				w(_q.w), 
				z(_q.z)
			{};


			inline Quaternion& operator*= (const Quaternion& _q)
			{
				//  w = (w1w2 - z1z2)
				//  z = (w1z2 + z1w2)
				w = w * _q.w - z * _q.z;
				z = w * _q.z - z * _q.w;
				return *this;
			};


			inline Quaternion operator* (const Quaternion& _q) const 
			{
				Quaternion tmp(*this);
				return tmp *= _q;
			};


			inline void rotate(float _t)
			{
				*this *= Quaternion(_t);
			};


			void normalise()
			{
				float len = w * w + z * z;
				if (std::fabs(len - 1.0f) > EPSILON)
				{
					len = 1.0f / std::sqrt(len);
					w *= len;
					z *= len;
				}
			};


			Vector2 operator* (const Vector2& _v)
			{
				// based on nVidia SDK with 3d vetors
				//  x = vx - qz * vy * 2w - 2 * qz * qz * vx
				//  y = vy + qz * vx * 2w - 2 * qz * qz * vy

				return Vector2(
					_v.x - ( z * _v.y * 2 * w ) - ( 2 * z * z * _v.x ),
					_v.y + ( z * _v.x * 2 * w ) - ( 2 * z * z * _v.y )
				);

			};

			/*
			quaternion * vector is a vector rotationally offset by the quaternion
				quaternion * quaternion is a quaternion with both rotations combined, 'therefore:'
				rotation1 * rotation2 is a quaternion with both rotations, rotation1 is the orientation of the parent node and orientation2 is the orientation of the child node 'and'
			*/
		};
	}
}