#pragma once

#include "../ScriptNode.h"

#include "SingleSprite.h"

#include "../ShadingProgramManager.h"
#include "../ImageSpriteManager.h"



namespace core
{

	enum OBJECT_TYPE
	{
		OT_UNDEFINED,
		OT_SINGLE_SPRITE,
	};


	class ObjectFactory
	{
	public:

		_2d::SingleSpriteUPtr createSingleSprite(const std::string &_name, ScriptNodePtr _scriptNode);
		_2d::MovableObjectUPtr createObject(ScriptNodePtr _scriptNode);

	};
}