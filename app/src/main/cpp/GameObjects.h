#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/StringUtils.h"
#include "core/ControllerManager.h"

#include "core/_2d/PhysicsSystem.h"
#include "core/_2d/PhysicsAffectors.h"
#include "core/_2d/shapes/ShapeFactory.h"


#include "core/_2d/SceneManager.h"
#include "core/_2d/MovableObject.h"
#include "core/_2d/SingleSprite.h"
#include "core/_2d/AnimatedSprite.h"
#include "core/_2d/ParticleSystem.h"
#include "InstanceParticleEmitters.h"

#include "core/_2d/RigidObject.h"

#include "core/InputManager.h"
#include "core/TouchControls.h"

#include "core/Logger.h"



using namespace core;
using namespace core::_2d;


static constexpr char *entityNodeName = "entity_node";
static constexpr char *entityBoundsName = "entity_bounds";

static constexpr char *playerMainBodyName = "player_main";
static constexpr char *playerLaserHeadName = "laser_head";
static constexpr char *playerLaserBodyName = "laser_body";

static constexpr char *animationPlayerIdle = "idle";
static constexpr char *animationPlayerWalkRight = "walk_right";
static constexpr char *animationPlayerShoot = "shoot";

static constexpr char *animLaserHeadShooting = "shooting_loop";
static constexpr char *animLaserHeadHitWall = "wall_hit";



static constexpr char *entityGroupNodeName = "entity_group_node";

static constexpr char *gravityPhysAffectorName = "gravity";
static constexpr char *stageObjectName = "stage_object";
static constexpr char *stageGravityValueName = "gravity_strength";
static constexpr char *stageSize = "dimensions";


static constexpr char *rigidTypePlayer = "player";
static constexpr char *rigidTypeLaser = "laser";
static constexpr char *rigidTypeStage = "stage";
static constexpr char *rigidTypeEnemy = "enemy";







class Laser
{

};


class Enemy
{

};


class Upgrade
{

};


class Stage
{
protected:

	SceneManager *scene;

	float width, height;
	float gravityStrength;

	SceneNode *backgroundNode;
	MovableObject *baseObject;


	RigidObjectUPtr bounds;


	void resetStage();
	void setBackground(ScriptNodePtr _bgData);
	void setGravity(float _gravityStrength);
	void setupBounds(float _width, float _height);

public:

	void createStage(ScriptNodePtr _stageData)
	{
		resetStage();

		float gravity = std::stof(_stageData->getValue(stageGravityValueName));
		setGravity(gravity);

		ScriptNodeListPtr nodeList = _stageData->getChildList();
		ScriptNodeList::iterator findRes = std::find(nodeList->begin(),
				nodeList->end(),
				[] (auto it) { return !((*it)->getName().compare(entityNodeName)); } );
		assert(findRes != nodeList->end() && "no scene node data in the script");
		setBackground(*findRes);

		std::string stageSize = _stageData->getValue(stageSize);
		std::vector<std::string> sizeTok = tokenize(stageSize, ",");
		assert(sizeTok.size() == 2 && "tokenized size require 2 numbers");
		setupBounds(std::stof(sizeTok[0]), std::stof(sizeTok[1]));
	};

};


