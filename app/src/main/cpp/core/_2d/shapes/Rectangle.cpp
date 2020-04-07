#include "Rectangle.h"

namespace core
{
	namespace _2d
	{

		void Rectangle::updateAABB()
		{
			boundingBox = AxisAlignedBox(points[0], points[0]);
			boundingBox.merge(points[1]);
			boundingBox.merge(points[2]);
			boundingBox.merge(points[3]);
		};


		Rectangle::Rectangle(Vector2 _p0, Vector2 _p1, Vector2 _p2, Vector2 _p3) :
			Shape(),
			points{_p0, _p1, _p2, _p3}
		{
			updateAABB();
		};


		Rectangle::Rectangle(float _width, float _height, const Vector2 &_position, float _cwRotation) : Shape()
		{
			setRectangle(_width, _height, _position, _cwRotation);
		};


		Rectangle::~Rectangle()
		{};


		void Rectangle::setRectangle(float _width, float _height, const Vector2 &_position, float _cwRotation)
		{
			points[0] = Vector2(-std::abs(_width * 0.5f), std::abs(_height * 0.5f));
			points[1] = Vector2(std::abs(_width * 0.5f), std::abs(_height * 0.5f));

			if (_cwRotation > EPSILON)
			{
				float cr(std::cos(_cwRotation)), sr(std::sin(_cwRotation));

				points[0] = Vector2(cr * points[0].x + sr * points[0].y, -sr * points[0].x + cr * points[0].y);
				points[1] = Vector2(cr * points[1].x + sr * points[1].y, -sr * points[1].x + cr * points[1].y);
			}
			points[2] = (points[0] * -1.0f) + _position;
			points[3] = (points[1] * -1.0f) + _position;
			points[0] += _position;
			points[1] += _position;

			memcpy(points, points, sizeof(Vector2) * 4);

			updateAABB();
		};


		const Vector2 &Rectangle::getPoint(unsigned int _index) const
		{
			assert( _index < 4 || "incorrect index");
			return points[_index];
		};


		ShapePtr Rectangle::transform(const Matrix3 &_mx) const
		{
			return ShapePtr(new Rectangle(_mx * points[0], _mx * points[1], _mx * points[2], _mx * points[3]));
		};


		const AxisAlignedBox &Rectangle::getAABB() const
		{
			return boundingBox;
		};


		bool Rectangle::contains(const Vector2 &_point) const
		{
			Vector2 normalVector, vectorToPoint;

			for (unsigned int i = 0; i < 4; ++i)
			{
				normalVector = normalVectorCCW(points[(i + 1) % 4] - points[i]);
				vectorToPoint = _point - points[i];
				if (dotProduct(normalVector, vectorToPoint) >= 0)
					return false;
			}

			return true;
		};


		bool Rectangle::intersect(const Shape *_shape) const
		{
			return _shape->intersect(this);
		};


		bool Rectangle::intersect(const Circle *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool Rectangle::intersect(const Rectangle *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		bool Rectangle::intersect(const LineArea *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		float Rectangle::distance(const Vector2 &_point) const
		{
			return _2d::distance(this, _point);
		};


		float Rectangle::distance(const Shape *_shape) const
		{
			return _shape->distance(this);
		};


		float Rectangle::distance(const Circle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float Rectangle::distance(const Rectangle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float Rectangle::distance(const LineArea *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		Vector2 Rectangle::separatingAxisNormal(const Shape *_shape) const
		{
			return -(_shape->separatingAxisNormal(this));
		};


		Vector2 Rectangle::separatingAxisNormal(const Circle *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 Rectangle::separatingAxisNormal(const Rectangle *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};


		Vector2 Rectangle::separatingAxisNormal(const LineArea *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};
	}
}