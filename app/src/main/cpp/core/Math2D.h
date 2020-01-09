#pragma once

#include "Matrix3.h"
#include "Vector2.h"
#include "_2d/Quaternion.h"


namespace core
{

	//  by default V2 is treated like point in 2D
	Vector2 operator*(const Matrix3& _m, const Vector2& _v);

	//  transform 2D vector
	Vector2 transformVector(const Matrix3& _m, const Vector2& _v);

	//  default transformation goes scaling -> rotation -> translation
	Matrix3 affine2DMatrix(const Vector2 &_scale, const _2d::Quaternion &_rotation, const Vector2 &_translation);

	float determinant(const Matrix3 &_mx);

	Matrix3 inverse(const Matrix3 &_mx);


	inline float dotProduct(_2d::Quaternion _q0, _2d::Quaternion _q1)
	{
		return _q0.w * _q1.w + _q0.z * _q1.z;
	};


}

