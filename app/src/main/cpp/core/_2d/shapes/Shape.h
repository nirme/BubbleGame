#pragma once

#include <memory>
#include "../../Vector2.h"
#include "../AxisAlignedBox.h"

namespace core
{
	namespace _2d
	{
		class Shape;
		typedef std::unique_ptr<Shape> ShapePtr;

		class Circle;
		class Rectangle;
		class LineArea;


		class Shape
		{
		public:
			virtual ~Shape(){};

			virtual ShapePtr transform(const Matrix3 &_mx) const = 0;
			virtual const AxisAlignedBox &getAABB() const = 0;

			virtual bool contains(const Vector2 &_point) const = 0;
			virtual bool intersect(const Shape *_shape) const = 0;

			virtual bool intersect(const Circle *_shape) const = 0;
			virtual bool intersect(const Rectangle *_shape) const = 0;
			virtual bool intersect(const LineArea *_shape) const = 0;

			virtual float distance(const Vector2 &_point) const = 0;
			virtual float distance(const Shape *_shape) const = 0;
			virtual float distance(const Circle *_shape) const = 0;
			virtual float distance(const Rectangle *_shape) const = 0;
			virtual float distance(const LineArea *_shape) const = 0;

			virtual Vector2 separatingAxisNormal(const Shape *_shape) const = 0;
			virtual Vector2 separatingAxisNormal(const Circle *_shape) const = 0;
			virtual Vector2 separatingAxisNormal(const Rectangle *_shape) const = 0;
			virtual Vector2 separatingAxisNormal(const LineArea *_shape) const = 0;


		};
	}
}