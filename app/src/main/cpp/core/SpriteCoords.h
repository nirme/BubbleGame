#pragma once

#include "Vector2.h"
#include "Matrix3.h"
#include "Math2D.h"
#include "_2d/AxisAlignedBox.h"


namespace core
{
	struct SpriteCoords
	{
		enum SIDE
		{
			S_LEFT = 0x01,
			S_RIGHT = 0x02,
			S_TOP = 0x03,
			S_BOTTOM = 0x04,
		};

		union
		{
			float uvArray[8];
			Vector2 uvPoints[4];
		};

		static const SpriteCoords SPRITE_SQUARE;

		SpriteCoords(const SpriteCoords &_rhs = SpriteCoords::SPRITE_SQUARE);
		SpriteCoords(float _left, float _right, float _top, float _bottom);
		SpriteCoords(float _width, float _height);
		SpriteCoords(const Vector2 &_minV, const Vector2 &_maxV);
		SpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3);

		SpriteCoords& operator*=(const Vector2 &_mod);
		SpriteCoords& operator+=(const Vector2 &_mod);
		SpriteCoords& operator=(const SpriteCoords &_rhs);
		SpriteCoords& transform(const Matrix3& _m);

		_2d::AxisAlignedBox getBoundingAABB();

		void set(SIDE _side, float _value);
	};


	SpriteCoords operator*(const Matrix3& _m, const SpriteCoords& _v);


}
