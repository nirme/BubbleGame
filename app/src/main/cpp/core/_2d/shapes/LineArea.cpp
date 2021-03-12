#include "LineArea.h"

namespace core
{
	namespace _2d
	{

		void LineArea::updateAABB()
		{
			if (std::abs(a) < EPSILON) // horizontal
			{
				float horizontalLimit = -c/b;
				if (b > 0)
				{
					boundingBox = AxisAlignedBox(-std::numeric_limits<float>::infinity(),
											   -std::numeric_limits<float>::infinity(),
											   std::numeric_limits<float>::infinity(),
											   horizontalLimit);
				}
				else
				{
					boundingBox = AxisAlignedBox(-std::numeric_limits<float>::infinity(),
											   horizontalLimit,
											   std::numeric_limits<float>::infinity(),
											   std::numeric_limits<float>::infinity());
				}

				return;
			}

			if (std::abs(b) < EPSILON) // vertical
			{
				float verticalLimit = -c/a;
				if (a > 0)
				{
					boundingBox = AxisAlignedBox(-std::numeric_limits<float>::infinity(),
											   -std::numeric_limits<float>::infinity(),
											   verticalLimit,
											   std::numeric_limits<float>::infinity());
				}
				else
				{
					boundingBox = AxisAlignedBox(verticalLimit,
											   -std::numeric_limits<float>::infinity(),
											   std::numeric_limits<float>::infinity(),
											   std::numeric_limits<float>::infinity());
				}
				return;
			}

			boundingBox = AxisAlignedBox(AxisAlignedBox::R_INFINITE);
		};


		void LineArea::fromTwoPoints(const Vector2 &_from, const Vector2 &_to)
		{
			Vector2 v = _to - _from;
			float r = v.y / v.x;
			if (r <= 1.0f)
			{
				float s = v.x >= 0.0f ? 1.0f : -1.0f;

				a = -r * s;
				b = s;
				c = (r * _from.x - _from.y) * s;
			}
			else
			{
				float s = v.y <= 0.0f ? 1.0f : -1.0f;

				r = v.x / v.y;

				a = s;
				b = -r * s;
				c = (r * _from.y-_from.x) * s;
			}
		};


		LineArea::LineArea(const Vector2 &_from, const Vector2 &_to) : Shape()
		{
			assert(_from != _to && "two different point are required");
			fromTwoPoints(_from, _to);
			updateAABB();
		};


		LineArea::LineArea(float _a, float _b, float _c) : Shape(),
			a(_a), b(_b), c(_c)
		{
			assert((a != 0.0f || b != 0.0f) && "params a and b cannot be both 0");
			updateAABB();
		};


		LineArea::~LineArea()
		{};


		ShapePtr LineArea::clone()
		{
			return std::make_unique<LineArea>(a,b,c);
		};


		Vector3 LineArea::getParameters() const
		{
			return Vector3(a, b, c);
		};


		float LineArea::getParamA() const
		{
			return a;
		};


		float LineArea::getParamB() const
		{
			return b;
		};


		float LineArea::getParamC() const
		{
			return c;
		};


		ShapePtr LineArea::transform(const Matrix3 &_mx) const
		{
			Matrix3 invMx = inverse(_mx);
			return std::make_unique<LineArea>(
					a * invMx.m11 + b * invMx.m21 + c * invMx.m31,
					a * invMx.m12 + b * invMx.m22 + c * invMx.m32,
					a * invMx.m13 + b * invMx.m23 + c * invMx.m33);
		};


		const AxisAlignedBox &LineArea::getAABB() const
		{
			return boundingBox;
		};


		bool LineArea::contains(const Vector2 &_point) const
		{
			return (a * _point.x + b * _point.y + c) <= 0.0f;
		};


		bool LineArea::intersect(const Shape *_shape) const
		{
			return _shape->intersect(this);
		};


		bool LineArea::intersect(const Circle *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool LineArea::intersect(const Rectangle *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool LineArea::intersect(const LineArea *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool LineArea::intersect(const Pill *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		float LineArea::distance(const Vector2 &_point) const
		{
			return _2d::distance(this, _point);
		};


		float LineArea::distance(const Shape *_shape) const
		{
			return _shape->distance(this);
		};


		float LineArea::distance(const Circle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float LineArea::distance(const Rectangle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float LineArea::distance(const LineArea *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float LineArea::distance(const Pill *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		Vector2 LineArea::separatingAxisNormal(const Shape *_shape) const
		{
			return -(_shape->separatingAxisNormal(this));
		};


		Vector2 LineArea::separatingAxisNormal(const Circle *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 LineArea::separatingAxisNormal(const Rectangle *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 LineArea::separatingAxisNormal(const LineArea *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 LineArea::separatingAxisNormal(const Pill *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};

	}
}
