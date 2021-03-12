#include "ObjectFactory.h"
#include "../ScriptLoader.h"

namespace core
{

	_2d::SingleSpriteUPtr ObjectFactory::createSingleSprite(const std::string &_name, ScriptNodePtr _scriptNode)
	{
		// keep object as unique ptr until another object take resposibility for it
		_2d::SingleSpriteUPtr object = std::make_unique<_2d::SingleSprite>(_name);

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



	_2d::AnimatedSpriteUPtr ObjectFactory::createAnimatedSprite(const std::string &_name, ScriptNodePtr _scriptNode)
	{
		_2d::AnimatedSpriteUPtr object = std::make_unique<_2d::AnimatedSprite>(_name);

		if (!_scriptNode)
			return object;

		ScriptLoader &scriptLoader = ScriptLoader::getSingleton();
		ImageSpriteManager &spriteManager = ImageSpriteManager::getSingleton();


		ScriptNodeListPtr animationNodeList = scriptLoader.getAnimationNodesList(_scriptNode);

		for (ScriptNodeListIterator it = animationNodeList->begin(); it != animationNodeList->end(); ++it)
		{
			std::map<float, std::string> sortedKeyframes;

			ScriptNodeListPtr keyframeNodeList = scriptLoader.getKeyframeNodeList(*it);
			for (ScriptNodeListIterator itFrame = keyframeNodeList->begin(); itFrame != keyframeNodeList->end(); ++itFrame)
			{
				sortedKeyframes.insert({
											   scriptLoader.parseKeyframeTime(*itFrame),
											   scriptLoader.parseKeyframeSpriteName(*itFrame)
									   });
			}

			std::vector<std::pair<float, ImageSpritePtr>> keyframes;
			keyframes.reserve(sortedKeyframes.size());
			for (auto itMap = sortedKeyframes.begin(), itMapEnd = sortedKeyframes.end(); itMap != itMapEnd; ++itMap)
			{
				keyframes.push_back({itMap->first, spriteManager.getByName(itMap->second)});
			}

			if (!object->getMaterial())
				object->setMaterial(
						ShadingProgramManager::getSingleton().getByName(scriptLoader.parseObjectShader(_scriptNode)),
						keyframes[0].second->getTexture()
				);

			object->addAnimation(
					scriptLoader.parseAnimationName(*it),
					keyframes,
					scriptLoader.parseAnimationLength(*it));
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		object->setDefaultAnimation(scriptLoader.parseDefaultAnimationName(_scriptNode), scriptLoader.parseDefaultAnimationMode(_scriptNode));

		object->setScale(scriptLoader.parseObjectScale(_scriptNode));
		object->setRotation(scriptLoader.parseObjectRotation(_scriptNode));
		object->setPosition(scriptLoader.parseObjectPosition(_scriptNode));

		object->setPriority(scriptLoader.parseRenderablePriority(_scriptNode));

		return object;

	};


	_2d::ParticleSystemUPtr ObjectFactory::createParticleSystem(const std::string &_name, ScriptNodePtr _scriptNode)
	{
		// keep object as unique ptr until another object take resposibility for it
		_2d::ParticleSystemUPtr object = std::make_unique<_2d::ParticleSystem>(_name);

		if (!_scriptNode)
			return object;

		ScriptLoader &scriptLoader = ScriptLoader::getSingleton();

		std::list<std::string> spriteNames = scriptLoader.parseParticlesImageSprites(_scriptNode);

		auto it = spriteNames.begin();
		ImageSpritePtr sprite = ImageSpriteManager::getSingleton().getByName((*it));
		object->setMaterial(
				ShadingProgramManager::getSingleton().getByName(scriptLoader.parseObjectShader(_scriptNode)),
				sprite->getTexture()
		);
		object->addSprite(sprite);

		for (++it; it != spriteNames.end(); ++it)
		{
			object->addSprite(ImageSpriteManager::getSingleton().getByName((*it)));
		}

		object->setScale(scriptLoader.parseObjectScale(_scriptNode));
		object->setRotation(scriptLoader.parseObjectRotation(_scriptNode));
		object->setPosition(scriptLoader.parseObjectPosition(_scriptNode));

		object->setPriority(scriptLoader.parseRenderablePriority(_scriptNode));


		//add affectors and emitters

		return object;
	};


	_2d::SpritedTextUPtr ObjectFactory::createSpritedText(const std::string &_name, ScriptNodePtr _scriptNode)
	{
		// keep object as unique ptr until another object take resposibility for it
		_2d::SpritedTextUPtr object = std::make_unique<_2d::SpritedText>(_name);

		if (!_scriptNode)
			return object;

		ScriptLoader &scriptLoader = ScriptLoader::getSingleton();

		object->setMaterial(
				ShadingProgramManager::getSingleton().getByName(scriptLoader.parseObjectShader(_scriptNode)),
				SpritedFontManager::getSingleton().getByName(scriptLoader.parseObjectFont(_scriptNode))
		);

		object->setScale(scriptLoader.parseObjectScale(_scriptNode));
		object->setRotation(scriptLoader.parseObjectRotation(_scriptNode));
		object->setPosition(scriptLoader.parseObjectPosition(_scriptNode));

		object->setPriority(scriptLoader.parseRenderablePriority(_scriptNode));

		object->setAnchorPosition(scriptLoader.parseTextAnchorPosition(_scriptNode));
		object->setText(scriptLoader.parseObjectText(_scriptNode));

		return object;
	};


	_2d::MovableObjectUPtr ObjectFactory::createObject(ScriptNodePtr _scriptNode)
	{
		throw std::logic_error("unimplemented");

		ScriptLoader &scriptLoader = ScriptLoader::getSingleton();
		std::string name = scriptLoader.parseObjectName(_scriptNode);
		OBJECT_TYPE type = scriptLoader.parseRenderableType(_scriptNode);

		switch (type)
		{
			case OT_SINGLE_SPRITE:
				return createSingleSprite(name, _scriptNode);

			case OT_ANIMATED_SPRITE:
				return createAnimatedSprite(name, _scriptNode);

			case OT_PARTICLE_SYSTEM:
				return createParticleSystem(name, _scriptNode);

			case OT_SPRITED_FONT:
				return createSpritedText(name, _scriptNode);
		}

		return _2d::MovableObjectUPtr(nullptr);
	};

}