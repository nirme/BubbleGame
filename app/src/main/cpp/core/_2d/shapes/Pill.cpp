#include "Pill.h"


namespace core
{
	namespace _2d
	{

		void Pill::updateAABB()
		{
			Vector2 minP(a), maxP(a);

			if ( b.x > a.x )
				maxP.x = b.x;
			else
				minP.x = a.x;

			if ( b.y > a.y )
				maxP.y = b.y;
			else
				minP.y = a.y;

			boundingBox = AxisAlignedBox(minP - r, maxP + r);
		};


		Pill::Pill(const Vector2 &_position, float length, float _radius, float _cwRotation) :
				Shape()
		{
			Vector2 dir(std::cosf(_cwRotation), -std::sinf(_cwRotation));
			dir *= length * 0.5f;

			a = _position - dir;
			b = _position - dir;
			r = _radius;
		};


		Pill::Pill(const Vector2 &_a, const Vector2 &_b, float _r) :
				Shape(),
				a(_a),
				b(_b),
				r(_r)
		{};


		Pill::~Pill()
		{};


		ShapePtr Pill::clone()
		{
			return std::make_unique<Pill>(a, b, r);
		};


		const Vector2 &Pill::getPointA() const
		{
			return a;
		};


		const Vector2 &Pill::getPointB() const
		{
			return b;
		};


		float Pill::getRadius() const
		{
			return r;
		};


		Vector2 Pill::getPosition() const
		{
			return a + (b - a) * 0.5f;
		};


		float Pill::getLength() const
		{
			return (b - a).length();
		};


		ShapePtr Pill::transform(const Matrix3 &_mx) const
		{
			return std::make_unique<Pill>(_mx * a, _mx * b, (_mx * Vector2(r, 0)).length());
		};


		const AxisAlignedBox &Pill::getAABB() const
		{
			return boundingBox;
		};


		bool Pill::contains(const Vector2 &_point) const
		{
			Vector2 ap = _point - a;
			Vector2 ab = b - a;

			if (dotProduct(ab, ap) <= 0.0f)
				return (_point - a).length() - r;


			Vector2 bp = _point - b;
			float rr = r * r;

			//within circles
			if (ap.lengthSq() <= rr || bp.lengthSq() <= rr)
				return true;

			Vector2 ba = a - b;

			//outside section area
			if (dotProduct(ab, ap) < 0.0f || dotProduct(ba, bp) < 0.0f)
				return false;

			ab.normalize();
			Vector2 perpendicular = normalVectorCW(ab);

			// distance from section within r
			return dotProduct(perpendicular, ap) <= r;
		};


		bool Pill::intersect(const Shape *_shape) const
		{
			return _shape->intersect(this);
		};


		bool Pill::intersect(const Circle *_shape) const
		{
			return distance(_shape->getPosition()) <= _shape->getRadius();
		};


		bool Pill::intersect(const LineArea *_shape) const
		{
			return _shape->distance(a) <= r || _shape->distance(b) <= r;
		};


		bool Pill::intersect(const Pill *_shape) const
		{
			return _2d::intersect(this, _shape);
		};


		float Pill::distance(const Vector2 &_point) const
		{
			Vector2 ap = _point - a;
			Vector2 ab = b - a;
			if (dotProduct(ap, ab) <= 0.0f)
				return ap.length() - r;

			Vector2 bp = _point - b;
			Vector2 ba = a - b;
			if (dotProduct(bp, ba) <= 0.0f)
				return bp.length() - r;

			return std::abs(dotProduct(normalVectorCW(ab), ap)) - r;
		};


		float Pill::distance(const Shape *_shape) const
		{
			return _shape->distance(this);
		};


		float Pill::distance(const Circle *_shape) const
		{
			return _2d::distance(this, _shape);
		};


		float Pill::distance(const LineArea *_shape) const
		{
			return _2d::distance(this, _shape);
		};

		float Pill::distance(const Pill *_shape) const
		{
			return _2d::distance(this, _shape);
		};
	}
}