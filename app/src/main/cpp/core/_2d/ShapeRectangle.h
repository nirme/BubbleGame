#pragma once

#include "Shape.h"

namespace core
{
	namespace _2d
	{
		class Rectangle : public Shape
		{
			Vector2 points[4];

		public:

			Rectangle(Vector2 _p1, Vector2 _p2, Vector2 _p3, Vector2 _p4) :
				points{ _p1, _p2, _p3, _p4}
			{};

			bool contains(const Vector2 &_point) const
			{
				unsigned int crossPoints = 0;

				Vector2 vDist;
				float yConst, scale, xPoint;

				for (unsigned int i = 0; i < 4; ++i)
				{
					if (_point == points[i])
						return true;

					vDist = points[((i + 1) % 4)] - points[i];

					yConst = _point.y - points[(i % 4)].y;
					scale = yConst / vDist.y;
					xPoint = points[i].x + (vDist.x * scale);

					// on line
					if (xPoint == _point.x)
						return true;

					if (scale >= 0.0f && scale <= 1.0f && xPoint > _point.x)
						++crossPoints;
				}

				return (crossPoints % 2) == 1;
			};

			//virtual bool contains(const Shape &_shape) const = 0;
			//virtual bool intersect(const Shape &_shape) const = 0;
			//virtual const AxisAlignedBox &getAABB(const Shape &_shape) const = 0;
		};
	}
}