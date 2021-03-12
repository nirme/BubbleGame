#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

#include "core/StringUtils.h"
#include "core/Logger.h"
#include "core/ControllerManager.h"

#include "core/_2d/SceneManager.h"

#include "core/_2d/PhysicsSystem.h"
#include "core/_2d/PhysicsAffectors.h"

#include "core/_2d/MovableObject.h"
#include "core/_2d/SingleSprite.h"
#include "core/_2d/AnimatedSprite.h"
#include "core/_2d/ParticleSystem.h"
#include "InstanceParticleEmitters.h"

#include "core/_2d/RigidObject.h"
#include "core/_2d/shapes/ShapeFactory.h"


#include "GameStrings.h"


using namespace core;
using namespace core::_2d;

class Stage
{
protected:

	static constexpr char *stageSceneNodeName = "scene_node";


	static constexpr char *stageWidthValueName = "width";
	static constexpr char *stageHeightValueName = "height";

	static constexpr char *stageGravityValueName = "gravity";


	static constexpr char *bgSpriteNodeName = "background_sprite";
	static constexpr char *bgSpriteIsAnimatedValueName = "is_animated";

	static constexpr char *spriteIdleAnimationValue = "idle_loop";

	static constexpr unsigned int explosionParticleCount = 100;


	float width, height;
	float gravityStrength;

	SceneManager *scene;
	SceneNode *backgroundNode;

	bool isAnimated;
	SingleSprite *staticBackground;
	AnimatedSprite *animatedBackground;
	RigidObjectUPtr bounds;

	ParticleSystem *explosionSystem;

public:

	void create(SceneManager *_scene, ScriptNodePtr _data);
	void setupCallbacks() {};

	void pause();
	void resume();

	void playExplosion(const Vector2 &_position, float radius);

};
