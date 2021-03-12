#pragma once

#include <memory>
#include "Shape.h"
#include "ShapeMath.h"

namespace core
{
	namespace _2d
	{

		class Pill : public Shape
		{
		protected:
			Vector2 a, b;
			float r;

			AxisAlignedBox boundingBox;


			void updateAABB();

		public:

			Pill(const Vector2 &_position, float length, float _radius, float _cwRotation = 0.0f);
			Pill(const Vector2 &_a, const Vector2 &_b, float _r);
			~Pill();

			virtual ShapePtr clone();

			const Vector2 &getPointA() const;
			const Vector2 &getPointB() const;
			float getRadius() const;
			Vector2 getPosition() const;
			float getLength() const;

			virtual ShapePtr transform(const Matrix3 &_mx) const;
			virtual const AxisAlignedBox &getAABB() const;

			virtual bool contains(const Vector2 &_point) const;
			virtual bool intersect(const Shape *_shape) const;

			virtual bool intersect(const Circle *_shape) const;
			virtual bool intersect(const Rectangle *_shape) const
			{
				throw std::logic_error("unimplemented");
			};
			virtual bool intersect(const LineArea *_shape) const;
			virtual bool intersect(const Pill *_shape) const;

			virtual float distance(const Vector2 &_point) const;
			virtual float distance(const Shape *_shape) const;
			virtual float distance(const Circle *_shape) const;
			virtual float distance(const Rectangle *_shape) const
			{
				throw std::logic_error("unimplemented");
			};
			virtual float distance(const LineArea *_shape) const;
			virtual float distance(const Pill *_shape) const = 0;

			virtual Vector2 separatingAxisNormal(const Shape *_shape) const
			{
				throw std::logic_error("unimplemented");
			};

			virtual Vector2 separatingAxisNormal(const Circle *_shape) const
			{
				throw std::logic_error("unimplemented");
			};

			virtual Vector2 separatingAxisNormal(const Rectangle *_shape) const
			{
				throw std::logic_error("unimplemented");
			};

			virtual Vector2 separatingAxisNormal(const LineArea *_shape) const
			{
				throw std::logic_error("unimplemented");
			};


		};
	}
}
