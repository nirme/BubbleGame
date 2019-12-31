#include "ObjectFactory.h"
#include "../ScriptLoader.h"

namespace core
{

	_2d::SingleSpriteUPtr ObjectFactory::createSingleSprite(const std::string &_name, ScriptNodePtr _scriptNode)
	{
		// keep object as unique ptr until another object take resposibility for it
		_2d::SingleSpriteUPtr object(new _2d::SingleSprite(_name));

		if (!_scriptNode)
			return object;

		ScriptLoader &scriptLoader = ScriptLoader::getSingleton();

		object->setMaterial(
			ShadingProgramManager::getSingleton().getByName(scriptLoader.parseObjectShader(_scriptNode)),
			ImageSpriteManager::getSingleton().getByName(scriptLoader.parseObjectImageSprite(_scriptNode))
		);

		object->setScale(scriptLoader.parseObjectScale(_scriptNode));
		object->setRotation(scriptLoader.parseObjectRotation(_scriptNode));
		object->setPosition(scriptLoader.parseObjectPosition(_scriptNode));

		object->setPriority(scriptLoader.parseRenderablePriority(_scriptNode));
		object->setSpriteCoords(scriptLoader.parseSingleSpriteCoords(_scriptNode));


		return object;
	};

	_2d::MovableObjectUPtr ObjectFactory::createObject(ScriptNodePtr _scriptNode)
	{
		ScriptLoader &scriptLoader = ScriptLoader::getSingleton();
		std::string name = scriptLoader.parseObjectName(_scriptNode);
		OBJECT_TYPE type = scriptLoader.parseRenderableType(_scriptNode);

		switch (type)
		{
		case OT_SINGLE_SPRITE:
			return createSingleSprite(name, _scriptNode);
		}

		return _2d::MovableObjectUPtr(nullptr);
	};

}