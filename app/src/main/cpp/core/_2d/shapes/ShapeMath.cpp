#include "ShapeMath.h"

#include "Circle.h"
#include "Rectangle.h"
#include "LineArea.h"

namespace core
{
	namespace _2d
	{
		bool intersect(const Circle *_circle1, const Circle *_circle2)
		{
			Vector2 dist = _circle1->getPosition() - _circle2->getPosition();
			float minDist = _circle1->getRadius() + _circle2->getRadius();
			minDist *= minDist;

			return (dist.x * dist.x + dist.y * dist.y < minDist);

		};

		bool intersect(const Circle *_circle, const Rectangle *_rect)
		{
			// change to counting only 2 normals as there are 2 parallel pairs that can use the same norm just reversed
			Vector2 lineNormalOutside, pointToCCenter;
			for (unsigned int i = 0; i < 4; ++i)
			{
				lineNormalOutside = normalize(_rect->getPoint((i + 1) % 4) - _rect->getPoint(i));
				lineNormalOutside = Vector2(-lineNormalOutside.y, lineNormalOutside.x);
				pointToCCenter = _circle->getPosition() - _rect->getPoint(i);

				if (dotProduct(lineNormalOutside, pointToCCenter) >= _circle->getRadius())
					return false;
			}

			return true;
		};

		bool intersect(const Circle *_circle, const LineArea *_line)
		{
			float a(_line->getParamA()), b(_line->getParamB());
			const Vector2 &circlePos = _circle->getPosition();
			float distance = (a * circlePos.x + b * circlePos.y + _line->getParamC()) / sqrt(a * a + b * b);

			return distance < _circle->getRadius();
		};

		bool intersect(const Rectangle *_rect, const Circle *_circle)
		{
			return intersect(_circle, _rect);
		};

		bool intersect(const Rectangle *_rect1, const Rectangle *_rect2)
		{
			// separating axis theorem
			Vector2 axisNormal, pointDistVector;
			bool axisFlag;

			for (unsigned int r = 0; r < 2; ++r)
			{
				for (unsigned int i = 0; i < 4; ++i)
				{
					axisNormal = normalVectorCCW(_rect1->getPoint((i + 1) % 4) - _rect1->getPoint(i));
					axisFlag = true;

					for (unsigned int j = 0; j < 4; ++j)
					{
						pointDistVector = _rect2->getPoint(j) - _rect1->getPoint(i);
						if (dotProduct(axisNormal, pointDistVector) < 0.0f)
						{
							axisFlag = false;
							break;
						}
					}

					if (axisFlag)
						return false;
				}

				std::swap(_rect1, _rect2);
			}

			return true;
		};

		bool intersect(const Rectangle *_rect, const LineArea *_line)
		{
			Vector2 lineNormal(_line->getParamA(), _line->getParamB());
			Vector2 linePoint = _rect->getPoint(0);

			if (abs(_line->getParamA()) > abs(_line->getParamB()))
				linePoint.x = -(_line->getParamB() * linePoint.y + _line->getParamC())/_line->getParamA();
			else
				linePoint.y = -(_line->getParamA() * linePoint.x + _line->getParamC())/_line->getParamB();

			for (unsigned int i = 0; i < 4; ++i)
			{
				Vector2 lToPVector = _rect->getPoint(i) - linePoint;
				if (dotProduct(lineNormal, lToPVector) < 0.0f)
					return true;
			}

			return false;
		};

		bool intersect(const LineArea *_line, const Circle *_circle)
		{
			return intersect(_circle, _line);
		};

		bool intersect(const LineArea *_line, const Rectangle *_rect)
		{
			return intersect(_rect, _line);
		};

		bool intersect(const LineArea *_line1, const LineArea *_line2)
		{
			if ((_line1->getParamA() + _line2->getParamA()) < EPSILON &&
				(_line1->getParamB() + _line2->getParamB()) < EPSILON &&
				(_line1->getParamC() + _line2->getParamC()) > 0.0f)
				return false;
			return true;
		};


		float distance(const Circle *_circle, const Vector2 &_point)
		{
			return (_circle->getPosition() - _point).length() - _circle->getRadius();
		};


		float distance(const Rectangle *_rect, const Vector2 &_point)
		{
			Vector2 p1ToP2[4];
			float denormDistance[4];
			float distFromP1, p1p2Len;
			bool inside = true;
			for (unsigned int i = 0; i < 4; ++i)
			{
				p1ToP2[i] = _rect->getPoint((i + 1) % 4) - _rect->getPoint(i);
				denormDistance[i] = dotProduct(normalVectorCCW(p1ToP2[i]), _point - _rect->getPoint(i));

				if (denormDistance[i] > 0.0f)
					inside = false;
			}

			if (inside)
			{
				for (unsigned int i = 0; i < 4; ++i)
					denormDistance[i] /= p1ToP2[i].length();

				return *std::max_element(denormDistance, denormDistance + 4);
			}

			unsigned int iRight;
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (denormDistance[i] > 0.0)
				{
					p1p2Len = p1ToP2[i].x * p1ToP2[i].x + p1ToP2[i].y * p1ToP2[i].y;
					distFromP1 = dotProduct(p1ToP2[i], _point - _rect->getPoint(i));

					if (distFromP1 >= 0.0f && distFromP1 <= p1p2Len)
						return denormDistance[i] / p1ToP2[i].length();
					else if (distFromP1 < 0.0f)
						iRight = i;
				}
			}

			return (_rect->getPoint(iRight) - _point).length();
		};


		float distance(const LineArea *_line, const Vector2 &_point)
		{
			float a(_line->getParamA()), b(_line->getParamB());
			return (a * _point.x + b * _point.y + _line->getParamC()) / sqrt(a * a + b * b);
		};


		float distance(const Circle *_circle1, const Circle *_circle2)
		{
			return (_circle1->getPosition() - _circle2->getPosition()).length() - _circle1->getRadius() - _circle2->getRadius();
		};


		float distance(const Circle *_circle, const Rectangle *_rect)
		{
			return distance(_rect, _circle->getPosition()) - _circle->getRadius();
		};


		float distance(const Circle *_circle, const LineArea *_line)
		{
			return distance(_line, _circle->getPosition()) - _circle->getRadius();
		};


		float distance(const Rectangle *_rect, const Circle *_circle)
		{
			return distance(_circle, _rect);
		};


		float distance(const Rectangle *_rect1, const Rectangle *_rect2)
		{
			float dist[8] = {
					distance(_rect1, _rect2->getPoint(0)),
					distance(_rect1, _rect2->getPoint(1)),
					distance(_rect1, _rect2->getPoint(2)),
					distance(_rect1, _rect2->getPoint(3)),
					distance(_rect2, _rect1->getPoint(0)),
					distance(_rect2, _rect1->getPoint(1)),
					distance(_rect2, _rect1->getPoint(2)),
					distance(_rect2, _rect1->getPoint(3))
			};

			return *std::min_element(dist, dist + 8);
		};


		float distance(const Rectangle *_rect, const LineArea *_line)
		{
			float dist[4] = {
					distance(_line, _rect->getPoint(0)),
					distance(_line, _rect->getPoint(1)),
					distance(_line, _rect->getPoint(2)),
					distance(_line, _rect->getPoint(3))
			};

			return *std::min_element(dist, dist + 4);
		};


		float distance(const LineArea *_line, const Circle *_circle)
		{
			return distance(_circle,_line);
		};


		float distance(const LineArea *_line, const Rectangle *_rect)
		{
			return distance(_rect,_line);
		};


		float distance(const LineArea *_line1, const LineArea *_line2)
		{
			return intersect(_line1, _line2) ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
		};


		Vector2 separatingAxisNormal(const Circle *_circle1, const Circle *_circle2)
		{
			return normalize((_circle2->getPosition() - _circle1->getPosition()));
		};


		Vector2 separatingAxisNormal(const Circle *_circle, const Rectangle *_rect)
		{
			return -separatingAxisNormal(_rect, _circle);
		};


		Vector2 separatingAxisNormal(const Rectangle *_rect, const Circle *_circle)
		{
			Vector2 sideVector;
			Vector2 sideNormal;
			Vector2 distanceVector;
			float posProj;
			int positionInd[4] = {9,9,9,9};
			float posDist[4];

			for (unsigned int i = 0; i < 4; ++i)
			{
				sideVector = _rect->getPoint((i+1)%4) - _rect->getPoint(i);
				sideNormal = normalVectorCCW(sideVector);
				distanceVector = _circle->getPosition() - _rect->getPoint(i);
				posDist[i] = dotProduct(sideNormal, distanceVector);

				if (posDist[i] >= 0.0f)
				{
					posProj = dotProduct(sideVector, distanceVector);
					positionInd[i] = posProj < 0.0f ? -1 : (posProj > dotProduct(sideVector, sideVector) ? 1.0f : 0.0f);
				}
			}

			for (unsigned int i = 0; i < 4; ++i)
			{
				if (positionInd[i] == 0)
					return normalize(normalVectorCCW(_rect->getPoint((i+1)%4) - _rect->getPoint(i)));

				if (positionInd[i] == -1 && positionInd[(4+i-1)%4] == 1)
					return normalize(_circle->getPosition() - _rect->getPoint(i));
			}

			const Vector2 &a = _rect->getPoint(0), &b = _rect->getPoint(1), &c = _rect->getPoint(2), &d = _rect->getPoint(3);
			Vector2 ac = c - a;
			Vector2 bd = d - b;

			float acRatio = ac.y / ac.x;
			float bdRatio = bd.y / bd.x;
			float m = ((((bdRatio * b.x) - b.y - (acRatio * a.x) + a.y) / (bdRatio - acRatio)) - a.x) / ac.x;

			Vector2 center(a + (ac * m));

			return normalize(_circle->getPosition() - center);
		};


		Vector2 separatingAxisNormal(const Circle *_circle, const LineArea *_line)
		{
			return -separatingAxisNormal(_line, _circle);
		};


		Vector2 separatingAxisNormal(const LineArea *_line, const Circle *_circle)
		{
			return normalize(Vector2(_line->getParamA(), _line->getParamB()));
		};


		Vector2 separatingAxisNormal(const Rectangle *_rect1, const Rectangle *_rect2)
		{
			Vector2 sideVector,  pointDistance;
			float sideLengthSq, pointDistSq, tmp;
			unsigned int distance = std::numeric_limits<float>::infinity();
			unsigned int min = -1;

			for (unsigned int i=0; i<4; ++i)
			{
				sideVector = _rect1->getPoint((i + 1) % 4) - _rect1->getPoint(i);
				sideLengthSq = dotProduct(sideVector, sideVector);

				for (unsigned int j=0; j<4; ++j)
				{
					pointDistance = _rect2->getPoint(j) - _rect1->getPoint(i);
					pointDistSq = dotProduct(pointDistance, sideVector);

					if (pointDistSq < 0 && pointDistSq > sideLengthSq)
					{
						tmp = dotProduct(pointDistance, normalVectorCCW(sideVector));
						if (tmp >= 0.0f)
						{
							tmp /= sideLengthSq;
							if (distance > tmp)
							{
								distance = tmp;
								min = i;
							}
						}
					}
				}
			}

			for (unsigned int i=0; i<4; ++i)
			{
				sideVector = _rect2->getPoint((i + 1) % 4) - _rect2->getPoint(i);
				sideLengthSq = dotProduct(sideVector, sideVector);

				for (unsigned int j=0; j<4; ++j)
				{
					pointDistance = _rect1->getPoint(j) - _rect2->getPoint(i);
					pointDistSq = dotProduct(pointDistance, sideVector);

					if (pointDistSq < 0 && pointDistSq > sideLengthSq)
					{
						tmp = dotProduct(pointDistance, normalVectorCCW(sideVector));
						if (tmp >= 0.0f)
						{
							tmp /= sideLengthSq;
							if (distance > tmp)
							{
								distance = tmp;
								min = 4 + i;
							}
						}
					}
				}
			}

			// one of the sides will be the axis
			if (distance < std::numeric_limits<float>::infinity())
			{
				if (min < 4)
				{
					return normalize(normalVectorCCW(_rect1->getPoint((min+1)%4) - _rect1->getPoint(min)));
				}
				else
				{
					return -normalize(normalVectorCCW(_rect2->getPoint((min+1)%4) - _rect2->getPoint(min)));
				}
			}


			// too far away, treat both shapes like circles
			Vector2 center1, center2;
			{
				const Vector2 &a = _rect1->getPoint(0), &b = _rect1->getPoint(1), &c = _rect1->getPoint(2), &d = _rect1->getPoint(3);
				Vector2 ac = c - a;
				Vector2 bd = d - b;

				float acRatio = ac.y / ac.x;
				float bdRatio = bd.y / bd.x;
				float m = ((((bdRatio * b.x) - b.y - (acRatio * a.x) + a.y) / (bdRatio - acRatio)) - a.x) / ac.x;

				center1 = a + (ac * m);
			}
			{
				const Vector2 &a = _rect2->getPoint(0), &b = _rect2->getPoint(1), &c = _rect2->getPoint(2), &d = _rect2->getPoint(3);
				Vector2 ac = c - a;
				Vector2 bd = d - b;

				float acRatio = ac.y / ac.x;
				float bdRatio = bd.y / bd.x;
				float m = ((((bdRatio * b.x) - b.y - (acRatio * a.x) + a.y) / (bdRatio - acRatio)) - a.x) / ac.x;

				center2 = a + (ac * m);
			}

			return normalize(center2 - center1);
		};


		Vector2 separatingAxisNormal(const LineArea *_line, const Rectangle *_rect)
		{
			return normalize(Vector2(_line->getParamA(), _line->getParamB()));
		};


		Vector2 separatingAxisNormal(const Rectangle *_rect, const LineArea *_line)
		{
			return -separatingAxisNormal(_line, _rect);
		};

		Vector2 separatingAxisNormal(const LineArea *_line1, const LineArea *_line2)
		{
			return normalize(normalize({_line2->getParamA(), _line2->getParamB()}) - normalize({_line1->getParamA(), _line1->getParamB()}));
		};

	}
}
