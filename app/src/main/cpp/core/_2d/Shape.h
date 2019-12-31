#pragma once

#include "../Vector2.h"
#include "AxisAlignedBox.h"

namespace core
{
	namespace _2d
	{
		class Shape
		{
		public:
			virtual ~Shape(){};
			virtual bool contains(const Vector2 &_point) const = 0;
			//virtual bool contains(const Shape &_shape) const = 0;
			//virtual bool intersect(const Shape &_shape) const = 0;
			//virtual const AxisAlignedBox &getAABB(const Shape &_shape) const = 0;
		};
	}
}