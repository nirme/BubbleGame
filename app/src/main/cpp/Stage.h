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


using namespace core;
using namespace core::_2d;

static constexpr char *rigidTypePlayer = "player";
static constexpr char *rigidTypeLaser = "laser";
static constexpr char *rigidTypeStage = "stage";
static constexpr char *rigidTypeEnemy = "enemy";


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



	float width, height;
	float gravityStrength;

	SceneManager *scene;
	SceneNode *backgroundNode;

	bool isAnimated;
	SingleSprite *staticBackground;
	AnimatedSprite *animatedBackground;
	RigidObjectUPtr bounds;



	void resetStage();
	void setBackground(ScriptNodePtr _bgData);
	void setGravity(float _gravityStrength);
	void setupBounds(float _width, float _height);

public:

	void create(SceneManager *_scene, ScriptNodePtr _data);
	void setupCallbacks() {};


	void playAnimation();
	void pauseAnimation();

};
