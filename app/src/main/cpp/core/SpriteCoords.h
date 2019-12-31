#pragma once

#include "Vector2.h"
#include "Matrix3.h"
#include "Math2D.h"
#include "_2d/AxisAlignedBox.h"


namespace core
{
	struct SpriteCoords
	{
		union
		{
			float uvArray[8];
			Vector2 uvPoints[4];
		};

		static const SpriteCoords SPRITE_SQUARE;

		SpriteCoords(const SpriteCoords &_rhs = SpriteCoords::SPRITE_SQUARE);
		SpriteCoords(float _left, float _right, float _top, float _bottom);
		SpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3);

		SpriteCoords& operator*=(const Vector2 &_mod);
		SpriteCoords& operator+=(const Vector2 &_mod);
		SpriteCoords& operator=(const SpriteCoords &_rhs);
		SpriteCoords& transform(const Matrix3& _m);

		_2d::AxisAlignedBox getBoundingAABB();
	};


	SpriteCoords operator*(const Matrix3& _m, const SpriteCoords& _v);


}
