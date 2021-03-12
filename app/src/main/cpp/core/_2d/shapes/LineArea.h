#pragma once

#include "../../Vector3.h"
#include "Shape.h"
#include "ShapeMath.h"

namespace core
{
	namespace _2d
	{
		class Circle;
		class Rectangle;

		class LineArea : public Shape
		{
			//area below the line
			// ---->
			// /////

		protected:
			// ax + by + c <= 0
			float a, b, c;
			AxisAlignedBox boundingBox;

			void updateAABB();
			void fromTwoPoints(const Vector2 &_from, const Vector2 &_to);

		public:

			LineArea(const Vector2 &_from, const Vector2 &_to);
			LineArea(float _a, float _b, float _c);
			virtual ~LineArea();

			virtual ShapePtr clone();

			Vector3 getParameters() const;
			float getParamA() const;
			float getParamB() const;
			float getParamC() const;

			virtual ShapePtr transform(const Matrix3 &_mx) const;
			virtual const AxisAlignedBox &getAABB() const;

			virtual bool contains(const Vector2 &_point) const;
			virtual bool intersect(const Shape *_shape) const;

			virtual bool intersect(const Circle *_shape) const;
			virtual bool intersect(const Rectangle *_shape) const;
			virtual bool intersect(const LineArea *_shape) const;
			virtual bool intersect(const Pill *_shape) const;

			virtual float distance(const Vector2 &_point) const;
			virtual float distance(const Shape *_shape) const;
			virtual float distance(const Circle *_shape) const;
			virtual float distance(const Rectangle *_shape) const;
			virtual float distance(const LineArea *_shape) const;
			virtual float distance(const Pill *_shape) const;

			virtual Vector2 separatingAxisNormal(const Shape *_shape) const;
			virtual Vector2 separatingAxisNormal(const Circle *_shape) const;
			virtual Vector2 separatingAxisNormal(const Rectangle *_shape) const;
			virtual Vector2 separatingAxisNormal(const LineArea *_shape) const;
			virtual Vector2 separatingAxisNormal(const Pill *_shape) const;
		};

		typedef std::unique_ptr<LineArea> LineAreaPtr;
	}
}