#pragma once

#include "Vector2.h"
#include "Matrix3.h"


using namespace core;

inline Vector2 transformPoint(const Matrix3& _matrix, const Vector2& _point)
{
	return Vector2(
		_matrix.m11 * _point.x + _matrix.m12 * _point.y + _matrix.m13,
		_matrix.m21 * _point.x + _matrix.m22 * _point.y + _matrix.m23
	);
};

inline Vector2 transformVector(const Matrix3& _matrix, const Vector2& _vector)
{
	return Vector2(
		_matrix.m11 * _vector.x + _matrix.m12 * _vector.y,
		_matrix.m21 * _vector.x + _matrix.m22 * _vector.y
	);
};


