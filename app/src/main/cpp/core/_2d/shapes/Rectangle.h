#pragma once

#include <cassert>
#include "Shape.h"
#include "ShapeMath.h"

namespace core
{
	namespace _2d
	{
		class Circle;
		class LineArea;

		class Rectangle : public Shape
		{
		protected:

			// 0---1
			// |   |
			// 3---2

			Vector2 points[4];
			AxisAlignedBox boundingBox;

			void updateAABB();

			Rectangle(Vector2 _p0, Vector2 _p1, Vector2 _p2, Vector2 _p3);

		public:

			Rectangle(float _width, float _height, const Vector2 &_position = Vector2(0.0f, 0.0f), float _cwRotation = 0.0f);
			virtual ~Rectangle();

			void setRectangle(float _width, float _height, const Vector2 &_position = Vector2(0.0f, 0.0f), float _cwRotation = 0.0f);
			const Vector2 &getPoint(unsigned int _index) const;

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

		typedef std::unique_ptr<Rectangle> RectanglePtr;
	}
}