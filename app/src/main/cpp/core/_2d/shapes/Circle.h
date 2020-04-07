#pragma once

#include <stdlib.h>
#include <cassert>
#include "Shape.h"
#include "ShapeMath.h"

namespace core
{
	namespace _2d
	{
		class Rectangle;
		class LineArea;

		class Circle : public Shape
		{
		protected:
			Vector2 position;
			float radius;
			AxisAlignedBox boundingBox;

			void updateAABB();

		public:

			Circle(const Vector2 &_position, float _radius);
			~Circle();

			void setPosition(const Vector2 &_position);
			void setRadius(float _radius);
			const Vector2 &getPosition() const;
			float getRadius() const;

			virtual ShapePtr transform(const Matrix3 &_mx) const;
			virtual const AxisAlignedBox &getAABB() const;

			virtual bool contains(const Vector2 &_point) const;
			virtual bool intersect(const Shape *_shape) const;

			virtual bool intersect(const Circle *_shape) const;
			virtual bool intersect(const Rectangle *_shape) const;
			virtual bool intersect(const LineArea *_shape) const;

			virtual float distance(const Vector2 &_point) const;
			virtual float distance(const Shape *_shape) const;
			virtual float distance(const Circle *_shape) const;
			virtual float distance(const Rectangle *_shape) const;
			virtual float distance(const LineArea *_shape) const;

			virtual Vector2 separatingAxisNormal(const Shape *_shape) const;
			virtual Vector2 separatingAxisNormal(const Circle *_shape) const;
			virtual Vector2 separatingAxisNormal(const Rectangle *_shape) const;
			virtual Vector2 separatingAxisNormal(const LineArea *_shape) const;
		};
	}
}
