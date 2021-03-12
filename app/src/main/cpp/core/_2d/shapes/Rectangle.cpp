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


		Rectangle::Rectangle(float _bottom, float _left, float _top, float _right)
		{
			setRectangle(_right - _left, _top - _bottom, {(_right - _left) * 0.5f, (_top - _bottom) * 0.5f});
		};


		Rectangle::Rectangle(float _width, float _height, const Vector2 &_position, float _cwRotation) : Shape()
		{
			setRectangle(_width, _height, _position, _cwRotation);
		};


		Rectangle::Rectangle(const Rectangle &_rhs) :
				points{_rhs.points[0], _rhs.points[1], _rhs.points[2], _rhs.points[3]}
		{
			updateAABB();
		};


		Rectangle::~Rectangle()
		{};


		ShapePtr Rectangle::clone()
		{
			return std::make_unique<Rectangle>(*this);
		};


		void Rectangle::setRectangle(float _width, float _height, const Vector2 &_position, float _cwRotation)
		{
			points[0] = Vector2(-std::abs(_width * 0.5f), std::abs(_height * 0.5f));
			points[1] = Vector2(std::abs(_width * 0.5f), std::abs(_height * 0.5f));


			if (std::fmodf(std::abs(_cwRotation), 2.0f * M_PI) > EPSILON)
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
			return std::make_unique<Rectangle>(_mx * points[0], _mx * points[1], _mx * points[2], _mx * points[3]);
		};


		const AxisAlignedBox &Rectangle::getAABB() const
		{
			return boundingBox;
		};


		bool Rectangle::contains(const Vector2 &_point) const
		{
			Vector2 perpendicularVector, pointToPoint;

			for (unsigned int i = 0; i < 4; ++i)
			{
				perpendicularVector = normalVectorCCW(points[(i + 1) % 4] - points[i]);
				pointToPoint = _point - points[i];
				if (dotProduct(perpendicularVector, pointToPoint) >= 0)
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


		bool Rectangle::intersect(const Pill *_shape) const
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


		float Rectangle::distance(const Pill *_shape) const
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


		Vector2 Rectangle::separatingAxisNormal(const Pill *_shape) const
		{
			return _2d::separatingAxisNormal(this, _shape);
		};

	}
}