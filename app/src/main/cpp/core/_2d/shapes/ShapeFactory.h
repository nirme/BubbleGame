#pragma once

#include <memory>
#include "../../ScriptNode.h"
#include "../../SingletonTemplate.h"
#include "Shape.h"
#include "Circle.h"
#include "LineArea.h"
#include "Rectangle.h"
#include "Pill.h"

namespace core
{
	namespace _2d
	{
		enum SHAPE_TYPE
		{
			ST_UNDEFINED = 0x00,
			ST_CIRCLE = 0x01,
			ST_RECTANGLE = 0x02,
			ST_LINE_AREA = 0x03,
			ST_PILL = 0x04,
		};


		class ShapeFactory // : public Singleton<ShapeFactory>
		{
		protected:

			static ShapePtr createCircle(ScriptNodePtr _data);
			static ShapePtr createRectangle(ScriptNodePtr _data);
			static ShapePtr createLineArea(ScriptNodePtr _data);
			static ShapePtr createPill(ScriptNodePtr _data);

		public:
			static ShapePtr parseShape(ScriptNodePtr _data);

		};
	}
}
