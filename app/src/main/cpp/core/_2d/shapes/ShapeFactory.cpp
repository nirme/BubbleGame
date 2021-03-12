#include "ShapeFactory.h"
#include "../../ScriptLoader.h"


//template<>
//core::_2d::ShapeFactory* Singleton<core::_2d::ShapeFactory>::impl = nullptr;

namespace core
{
	namespace _2d
	{

		ShapePtr ShapeFactory::createCircle(ScriptNodePtr _data)
		{
			Vector2 position = ScriptLoader::getSingleton().parseShapePosition(_data);
			float radius = ScriptLoader::getSingleton().parseShapeRadius(_data);

			return std::make_unique<Circle>(position, radius)
		};

		ShapePtr ShapeFactory::createRectangle(ScriptNodePtr _data)
		{
			Vector2 size = ScriptLoader::getSingleton().parseRectangleSize(_data);
			Vector2 position = ScriptLoader::getSingleton().parseShapePosition(_data);
			float rotation = ScriptLoader::getSingleton().parseShapeRotation(_data);

			return std::make_unique<Rectangle>(size.x, size.y, position, rotation));
		};

		ShapePtr ShapeFactory::createLineArea(ScriptNodePtr _data)
		{
			Vector3 params = ScriptLoader::getSingleton().parse2DLineParams(_data);

			return std::make_unique<LineArea>(params.x, params.y, params.z));
		};


		ShapePtr ShapeFactory::createPill(ScriptNodePtr _data)
		{
			Vector2 position = ScriptLoader::getSingleton().parseShapePosition(_data);
			float length = ScriptLoader::getSingleton().parsePillLength(_data);
			float radius = ScriptLoader::getSingleton().parseShapeRadius(_data);
			float rotation = ScriptLoader::getSingleton().parseShapeRotation(_data);

			return std::make_unique<Pill>(position, length, radius, rotation);

		};


		ShapePtr ShapeFactory::parseShape(ScriptNodePtr _data)
		{
			assert(ScriptLoader::getSingletonPtr() && "script loader is not available");

			SHAPE_TYPE type = ScriptLoader::getSingleton().parseShapeType(_data);
			switch (type)
			{
				case ST_CIRCLE:
					return createCircle(_data);

				case ST_RECTANGLE:
					return createRectangle(_data);

				case ST_LINE_AREA:
					return createLineArea(_data);

				case ST_PILL:
					return createPill(_data);

				default:
					return nullptr;
			}

		};
	}
}
