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





class Upgrade
{

};



