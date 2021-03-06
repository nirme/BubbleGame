#pragma once

#include <exception>
#include <stdexcept>
#include <stdint.h>
#include <algorithm>
#include <string>
#include "../../Vector2.h"
#include "../AxisAlignedBox.h"


namespace core
{
	namespace _2d
	{
		class Circle;
		class Rectangle;
		class LineArea;
		class Pill;

		bool intersect(const Circle *_circle1, const Circle *_circle2);
		bool intersect(const Circle *_circle, const Rectangle *_rect);
		bool intersect(const Circle *_circle, const LineArea *_line);
		bool intersect(const Circle *_circle, const Pill *_pill);

		bool intersect(const Rectangle *_rect, const Circle *_circle);
		bool intersect(const Rectangle *_rect1, const Rectangle *_rect2);
		bool intersect(const Rectangle *_rect, const LineArea *_line);
		bool intersect(const Rectangle *_rect, const Pill *_pill);

		bool intersect(const LineArea *_line, const Circle *_circle);
		bool intersect(const LineArea *_line, const Rectangle *_rect);
		bool intersect(const LineArea *_line1, const LineArea *_line2);
		bool intersect(const LineArea *_line, const Pill *_pill);

		bool intersect(const Pill *_pill, const Circle *_circle);
		bool intersect(const Pill *_pill, const Rectangle *_rect);
		bool intersect(const Pill *_pill, const LineArea *_line);
		bool intersect(const Pill *_pill1, const Pill *_pill2);


		float distance(const Circle *_circle, const Vector2 &_point);
		float distance(const Rectangle *_rect, const Vector2 &_point);
		float distance(const LineArea *_line, const Vector2 &_point);
		float distance(const Pill *_pill, const Vector2 &_point);

		float distance(const Circle *_circle1, const Circle *_circle2);
		float distance(const Circle *_circle, const Rectangle *_rect);
		float distance(const Circle *_circle, const LineArea *_line);
		float distance(const Circle *_circle, const Pill *_pill);

		float distance(const Rectangle *_rect, const Circle *_circle);
		float distance(const Rectangle *_rect1, const Rectangle *_rect2);
		float distance(const Rectangle *_rect, const LineArea *_line);
		float distance(const Rectangle *_rect, const Pill *_pill);

		float distance(const LineArea *_line, const Circle *_circle);
		float distance(const LineArea *_line, const Rectangle *_rect);
		float distance(const LineArea *_line1, const LineArea *_line2);
		float distance(const LineArea *_line, const Pill *_pill);

		float distance(const Pill *_pill, const Circle *_circle);
		float distance(const Pill *_pill, const Rectangle *_rect);
		float distance(const Pill *_pill, const LineArea *_line);
		float distance(const Pill *_pill1, const Pill *_pill2);


		Vector2 separatingAxisNormal(const Circle *_circle1, const Circle *_circle2);

		Vector2 separatingAxisNormal(const Circle *_circle, const Rectangle *_rect);
		Vector2 separatingAxisNormal(const Rectangle *_rect, const Circle *_circle);

		Vector2 separatingAxisNormal(const Circle *_circle, const LineArea *_line);
		Vector2 separatingAxisNormal(const LineArea *_line, const Circle *_circle);

		Vector2 separatingAxisNormal(const Circle *_circle, const Pill *_pill);
		Vector2 separatingAxisNormal(const Pill *_pill, const Circle *_circle);


		Vector2 separatingAxisNormal(const Rectangle *_rect1, const Rectangle *_rect2);

		Vector2 separatingAxisNormal(const Rectangle *_rect, const LineArea *_line);
		Vector2 separatingAxisNormal(const LineArea *_line, const Rectangle *_rect);

		Vector2 separatingAxisNormal(const Rectangle *_rect, const Pill *_pill);
		Vector2 separatingAxisNormal(const Pill *_pill, const Rectangle *_rect);

		Vector2 separatingAxisNormal(const LineArea *_line1, const LineArea *_line2);

		Vector2 separatingAxisNormal(const LineArea *_line, const Pill *_pill);
		Vector2 separatingAxisNormal(const Pill *_pill, const LineArea *_line);

		Vector2 separatingAxisNormal(const Pill *_pill1, const Pill *_pill2);

	}
}
