#pragma once

#include <limits>
#include "../Vector2.h"
#include "../Matrix3.h"
#include "../Math2D.h"




namespace core
{
	namespace _2d
	{

		class AxisAlignedBox
		{
		public:
			enum Range
			{
				R_NONE = 0x00,
				R_BOX = 0x01,
				R_INFINITE = 0x02,
			};

			enum Corner
			{
				TOP_RIGHT = 0,
				BOTTOM_RIGHT = 1,
				BOTTOM_LEFT = 2,
				TOP_LEFT = 3,
			};

			//  3---0
			//  |   |
			//  2---1


		protected:
			Vector2 vMin;
			Vector2 vMax;

			Range boxRange;


		public:


			inline AxisAlignedBox(Range _range = R_NONE) :
				vMin(_range == R_BOX ? -0.5f : (_range == R_INFINITE ? -std::numeric_limits<float>::infinity() : 0.0f)),
				vMax(_range == R_BOX ? 0.5f : (_range == R_INFINITE ? std::numeric_limits<float>::infinity() : 0.0f)),
				boxRange(_range)
			{};

			inline AxisAlignedBox(float _minX, float _minY, float _maxX, float _maxY) :
				vMin(_minX, _minY), vMax(_maxX, _maxY), boxRange(R_BOX)
			{};

			inline AxisAlignedBox(const Vector2& _min, const Vector2& _max) :
				vMin(_min), vMax(_max), boxRange(R_BOX)
			{};

			inline AxisAlignedBox(const AxisAlignedBox &_box) :
				vMin(_box.getMinimum()),
				vMax(_box.getMaximum()),
				boxRange(_box.getRange())
			{};

			inline AxisAlignedBox& operator= (const AxisAlignedBox& _box)
			{
				vMin = _box.getMinimum();
				vMax = _box.getMaximum();
				boxRange = _box.getRange();
				return *this;
			};

			inline AxisAlignedBox& operator= (AxisAlignedBox&& _box)
			{
				vMin = _box.getMinimum();
				vMax = _box.getMaximum();
				boxRange = _box.getRange();
				return *this;
			};


			inline Range getRange() const
			{
				return boxRange;
			};

			inline bool isEmpty() const
			{
				return boxRange == R_NONE;
			};

			inline bool isInfinite() const
			{
				return boxRange == R_INFINITE;
			};

			inline bool isBox() const
			{
				return boxRange == R_BOX;
			};


			inline void setMinimum(const Vector2& _min)
			{
				boxRange = R_BOX;
				vMin = _min;
			};

			inline void setMaximum(const Vector2& _max)
			{
				boxRange = R_BOX;
				vMax = _max;
			};


			inline const Vector2& getMinimum() const
			{
				return vMin;
			};

			inline const Vector2& getMaximum() const
			{
				return vMax;
			};


			inline AxisAlignedBox& merge(const Vector2 &_point)
			{
				if (isEmpty())
				{
					this->vMin = _point;
					this->vMax = _point;
					this->boxRange = R_BOX;
					return *this;
				}

				if (isInfinite())
					return *this;

				if (vMin.x > _point.x)
					vMin.x = _point.x;
				else if (vMax.x < _point.x)
					vMax.x = _point.x;

				if (vMin.y > _point.y)
					vMin.y = _point.y;
				else if (vMax.y < _point.y)
					vMax.y = _point.y;

				return *this;
			};


			inline AxisAlignedBox& merge(const AxisAlignedBox &_box)
			{
				if (_box.isEmpty() || isInfinite())
					return *this;

				if (isEmpty() || _box.isInfinite())
				{
					*this = _box;
					return *this;
				}


				if (vMin.x > _box.getMinimum().x)	vMin.x = _box.getMinimum().x;
				if (vMin.y > _box.getMinimum().y)	vMin.y = _box.getMinimum().y;
				if (vMax.x < _box.getMaximum().x)	vMax.x = _box.getMaximum().x;
				if (vMax.y < _box.getMaximum().y)	vMax.y = _box.getMaximum().y;

				return *this;
			};


			static AxisAlignedBox merge(const AxisAlignedBox& _box1, const AxisAlignedBox& _box2)
			{
				if (_box1.isEmpty() && _box2.isEmpty())
					return AxisAlignedBox();

				if (_box1.isEmpty() || _box2.isInfinite())
					return _box2;

				if (_box2.isEmpty() || _box1.isInfinite())
					return _box1;

				return AxisAlignedBox(
					_box1.getMinimum().x < _box2.getMinimum().x ? _box1.getMinimum().x : _box2.getMinimum().x,
					_box1.getMinimum().y < _box2.getMinimum().y ? _box1.getMinimum().y : _box2.getMinimum().y,
					_box1.getMaximum().x < _box2.getMaximum().x ? _box1.getMaximum().x : _box2.getMaximum().x,
					_box1.getMaximum().y < _box2.getMaximum().y ? _box1.getMaximum().y : _box2.getMaximum().y
				);
			};


			AxisAlignedBox& move(const Vector2& _v)
			{
				if (isBox())
				{
					vMin + _v;
					vMax + _v;
				}
				return *this;
			};


			AxisAlignedBox& transform(const Matrix3& _m)
			{
				if (isBox())
				{
					Vector2 points[3] = {

						_m * vMax,
						_m * Vector2(vMin.x, vMax.y),
						_m * Vector2(vMax.x, vMin.y)
					};

					vMin = _m * vMin;
					vMax = vMin;

					vMin.x = points[0].x < vMin.x ?
						(points[1].x < points[0].x ?
						(points[2].x < points[1].x ?
							points[2].x : points[1].x
							) :
							points[0].x
							) :
						vMin.x;

					vMin.y = points[0].y < vMin.y ?
						(points[1].y < points[0].y ?
						(points[2].y < points[1].y ?
							points[2].y : points[1].y
							) :
							points[0].y
							) :
						vMin.y;


					vMax.x = points[0].x > vMax.x ?
						(points[1].x > points[0].x ?
						(points[2].x > points[1].x ?
							points[2].x : points[1].x
							) :
							points[0].x
							) :
						vMax.x;

					vMax.y = points[0].y > vMax.y ?
						(points[1].y > points[0].y ?
						(points[2].y > points[1].y ?
							points[2].y : points[1].y
							) :
							points[0].y
							) :
						vMax.y;
				}

				return *this;
			};

			inline static bool isOverlapping(const AxisAlignedBox& _box1, const AxisAlignedBox& _box2)
			{
				if (_box1.isBox() && _box2.isBox())
				{
					return _box1.vMin.x < _box2.vMax.x &&
							_box1.vMin.y < _box2.vMax.y &&
							_box2.vMin.x < _box1.vMax.x &&
							_box2.vMin.y < _box1.vMax.y;
				}

				if (_box1.isEmpty() || _box2.isEmpty())
					return false;

				//if (_box1.isInfinite() || _box2.isInfinite())
					return true;
			};

			inline bool isOverlapping(const AxisAlignedBox& _box) const
			{
				return isOverlapping(*this, _box);
			};

		};

	}
}
