#pragma once

#include "../ScriptNode.h"

#include "SingleSprite.h"
#include "ParticleSystem.h"
#include "SpritedText.h"
#include "AnimatedSprite.h"

#include "../ShadingProgramManager.h"
#include "../ImageSpriteManager.h"
#include "../ImageSpriteManager.h"
#include "../SpritedFontManager.h"



namespace core
{

	enum OBJECT_TYPE
	{
		OT_UNDEFINED,
		OT_SINGLE_SPRITE,
		OT_ANIMATED_SPRITE,
		OT_PARTICLE_SYSTEM,
		OT_SPRITED_FONT,
	};


	class ObjectFactory
	{
	public:

		_2d::SingleSpriteUPtr createSingleSprite(const std::string &_name, ScriptNodePtr _scriptNode);

		_2d::AnimatedSpriteUPtr createAnimatedSprite(const std::string &_name, ScriptNodePtr _scriptNode);

		_2d::ParticleSystemUPtr createParticleSystem(const std::string &_name, ScriptNodePtr _scriptNode);

		_2d::SpritedTextUPtr createSpritedText(const std::string &_name, ScriptNodePtr _scriptNode);

		_2d::MovableObjectUPtr createObject(ScriptNodePtr _scriptNode);
	};
}