#include "Circle.h"

namespace core
{
	namespace _2d
	{

		void Circle::updateAABB()
		{
			boundingBox.setMinimum(position - radius);
			boundingBox.setMaximum(position + radius);
		};


		Circle::Circle(const Vector2 &_position, float _radius) :
				Shape(),
				position(_position),
				radius(_radius),
				boundingBox(_position - _radius, _position + _radius)
		{
			updateAABB();
		};

		Circle::~Circle()
		{};


		ShapePtr Circle::clone()
		{
			return std::make_unique<Circle>(position, radius);
		};


		void Circle::setPosition(const Vector2 &_position)
		{
			position = _position;
		};


		void Circle::setRadius(float _radius)
		{
			radius = _radius;
		};


		const Vector2 &Circle::getPosition() const
		{
			return position;
		};


		float Circle::getRadius() const
		{
			return radius;
		};


		ShapePtr Circle::transform(const Matrix3 &_mx) const
		{
			//assert((std::abs(_mx.m11 + _mx.m12) - std::abs(_mx.m21 + _mx.m22)) < EPSILON && "circle cannot be transformed with different x/y scales");
			return std::make_unique<Circle>(_mx * position, Vector2(_mx.m11 * radius, _mx.m21 * radius).length());
		};


		const AxisAlignedBox &Circle::getAABB() const
		{
			return boundingBox;
		};


		bool Circle::contains(const Vector2 &_point) const
		{
			Vector2 distance = position - _point;
			return (distance.x * distance.x + distance.y * distance.y) <= (radius * radius);
		};


		bool Circle::intersect(const Shape *_shape) const
		{
			return _shape->intersect(this);
		};


		bool Circle::intersect(const Circle *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool Circle::intersect(const Rectangle *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool Circle::intersect(const LineArea *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool Circle::intersect(const Pill *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		float Circle::distance(const Vector2 &_point) const
		{
			return _2d::distance(this, _point);
		};


		float Circle::distance(const Shape *_shape) const
		{
			return _shape->distance(this);
		};


		float Circle::distance(const Circle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float Circle::distance(const Rectangle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float Circle::distance(const LineArea *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float Circle::distance(const Pill *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		Vector2 Circle::separatingAxisNormal(const Shape *_shape) const
		{
			return -(_shape->separatingAxisNormal(this));
		};


		Vector2 Circle::separatingAxisNormal(const Circle *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 Circle::separatingAxisNormal(const Rectangle *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 Circle::separatingAxisNormal(const LineArea *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 Circle::separatingAxisNormal(const Pill *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};

	}
}
