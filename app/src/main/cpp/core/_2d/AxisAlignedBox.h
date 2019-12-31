#pragma once

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
				//R_INFINITE = 0x02,
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
				vMin(_range == R_BOX ? -0.5f : 0.0f), vMax(_range == R_BOX ? 0.5f : 0.0f), boxRange(_range)
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

			bool isEmpty() const
			{
				return boxRange == R_NONE;
			};


			inline void setMinimum(const Vector2& _min)
			{
				vMin = _min;
			};

			inline void setMaximum(const Vector2& _max)
			{
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


			inline AxisAlignedBox& merge(const AxisAlignedBox &_box)
			{
				if (_box.isEmpty())
					return *this;

				if (isEmpty())
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
				if (!_box1.isEmpty() && !_box2.isEmpty())
					return AxisAlignedBox();

				if (_box1.isEmpty())
					return _box2;

				if (_box2.isEmpty())
					return _box1;

				return AxisAlignedBox(
					_box1.getMinimum().x < _box2.getMinimum().x ? _box1.getMinimum().x : _box2.getMinimum().x,
					_box1.getMinimum().y < _box2.getMinimum().y ? _box1.getMinimum().y : _box2.getMinimum().y,
					_box1.getMaximum().x < _box2.getMaximum().x ? _box1.getMaximum().x : _box2.getMaximum().x,
					_box1.getMaximum().y < _box2.getMaximum().y ? _box1.getMaximum().y : _box2.getMaximum().y
				);
			};



			AxisAlignedBox& transform(const Matrix3& _m)
			{
				if (!isEmpty())
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

			bool isOverlapping(const AxisAlignedBox& _box) const
			{
				if (isEmpty() || _box.isEmpty())
					return false;

				const Vector2 &vMin2 = _box.getMinimum();
				const Vector2 &vMax2 = _box.getMaximum();

				return vMin.x < vMax2.x &&
					vMax.x > vMin2.x &&
					vMin.y < vMax2.y &&
					vMax.y > vMin2.y;
			};



		};

	}
}
