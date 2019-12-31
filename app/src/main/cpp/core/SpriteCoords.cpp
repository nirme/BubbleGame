#include "SpriteCoords.h"

namespace core
{

	SpriteCoords::SpriteCoords(const SpriteCoords &_rhs) :
		uvArray{
			_rhs.uvArray[0], _rhs.uvArray[1],
			_rhs.uvArray[2], _rhs.uvArray[3],
			_rhs.uvArray[4], _rhs.uvArray[5],
			_rhs.uvArray[6], _rhs.uvArray[7],
		}
	{};


	SpriteCoords::SpriteCoords(float _left, float _right, float _top, float _bottom) :
		uvArray{
			_left, _top,
			_right, _top,
			_left, _bottom, 
			_right, _bottom
		}
	{};


	SpriteCoords::SpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3) :
		uvPoints{
			_v0,
			_v1,
			_v2,
			_v3
		}
	{};


	SpriteCoords& SpriteCoords::operator*=(const Vector2 &_mod)
	{
		uvArray[0] *= _mod.x;
		uvArray[2] *= _mod.x;
		uvArray[4] *= _mod.x;
		uvArray[6] *= _mod.x;

		uvArray[1] *= _mod.y;
		uvArray[3] *= _mod.y;
		uvArray[5] *= _mod.y;
		uvArray[7] *= _mod.y;

		return *this;
	};


	SpriteCoords& SpriteCoords::operator+=(const Vector2 &_mod)
	{
		uvArray[0] += _mod.x;
		uvArray[2] += _mod.x;
		uvArray[4] += _mod.x;
		uvArray[6] += _mod.x;

		uvArray[1] += _mod.y;
		uvArray[3] += _mod.y;
		uvArray[5] += _mod.y;
		uvArray[7] += _mod.y;

		return *this;
	};


	SpriteCoords& SpriteCoords::operator=(const SpriteCoords &_rhs)
	{
		memcpy(uvArray, _rhs.uvArray, sizeof(uvArray));
		return *this;
	};


	SpriteCoords& SpriteCoords::transform(const Matrix3& _m)
	{
		uvPoints[0] = _m * uvPoints[0];
		uvPoints[1] = _m * uvPoints[1];
		uvPoints[2] = _m * uvPoints[2];
		uvPoints[3] = _m * uvPoints[3];
		return *this;
	};


	_2d::AxisAlignedBox SpriteCoords::getBoundingAABB()
	{
		Vector2 min(
				uvPoints[0].x < uvPoints[1].x ? uvPoints[0].x : uvPoints[1].x,
				uvPoints[0].y < uvPoints[1].y ? uvPoints[0].y : uvPoints[1].y
		);

		Vector2 max(
				uvPoints[0].x > uvPoints[1].x ? uvPoints[0].x : uvPoints[1].x,
				uvPoints[0].y > uvPoints[1].y ? uvPoints[0].y : uvPoints[1].y
		);


		if		(uvPoints[2].x < min.x)		min.x = uvPoints[2].x;
		else if	(uvPoints[2].x > max.x)		max.x = uvPoints[2].x;

		if		(uvPoints[2].y < min.y)		min.y = uvPoints[2].y;
		else if	(uvPoints[2].y > max.y)		max.y = uvPoints[2].y;


		if		(uvPoints[3].x < min.x)		min.x = uvPoints[3].x;
		else if	(uvPoints[3].x > max.x)		max.x = uvPoints[3].x;

		if		(uvPoints[3].y < min.y)		min.y = uvPoints[3].y;
		else if	(uvPoints[3].y > max.y)		max.y = uvPoints[3].y;

		return _2d::AxisAlignedBox(min, max);
	};


	const SpriteCoords SpriteCoords::SpriteCoords::SPRITE_SQUARE = SpriteCoords(
		{ -0.5f, 0.5f },
		{ 0.5f, 0.5f },
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f }
	);


	SpriteCoords operator*(const Matrix3& _m, const SpriteCoords& _v)
	{
		return SpriteCoords(
			_m * _v.uvPoints[0],
			_m * _v.uvPoints[1],
			_m * _v.uvPoints[2],
			_m * _v.uvPoints[3]
		);
	};

}
