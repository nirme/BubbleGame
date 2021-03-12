#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

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

#include "core/InputManager.h"
#include "core/TouchControls.h"

#include "core/StringUtils.h"


#include "GameStrings.h"


using namespace core;
using namespace core::_2d;

class Player
{
public:

	class Listener
	{
	public:
		virtual void onEnemyHit() = 0;
		virtual ~Listener(){};

	};

protected:

	static constexpr char *playerSceneNodeName = "player_node";

	static constexpr char *accelerationValueName = "acceleration";
	static constexpr char *maximumSpeedValueName = "maximum_speed";
	static constexpr char *laserExtensionSpeedValueName = "laser_extension_speed";
	static constexpr char *laserPositionOffsetValueName = "laser_position_offset";

	static constexpr char *bodySpriteNodeName = "body_sprite";
	static constexpr char *laserHeadNodeName = "laser_head";
	static constexpr char *laserNodeName = "laser";
	static constexpr char *laserParticleSystemNodeName = "laser_particle_system";

	static constexpr char *playerBoundsNodeName = "player_bounds";
	static constexpr char *laserBoundsNodeName = "laser_bounds";



	static constexpr char *movementAffector = "player_affector";
	static constexpr PhysicsSpeedAffector::ModifierOperation movementAffectorOperation = PhysicsSpeedAffector::MO_MUL;
	static constexpr float movementAffectorFactor = 0.8f;
	static constexpr float invulnerabilityTimeAfterEnemyHit = 3.0f; // in seconds
	static constexpr float invulnerabilityFlickerTime  = 0.5f;


	static constexpr char *animationPlayerIdle = "idle";
	static constexpr char *animationPlayerWalk = "walk";
	static constexpr char *animationLaserHeadShooting = "fire_loop";
	//static constexpr char *animationLaserHeadAgaintsWall = "wall_hit_loop";


	static constexpr unsigned int inputLeftControlIndex = 0;
	static constexpr unsigned int inputRightControlIndex = 1;
	static constexpr unsigned int inputShootingControlIndex = 2;


	static constexpr char *animationShootingStart = "shooting_start";
	static constexpr char *animationShootingEnd = "shooting_end";
	static constexpr unsigned int animationShootingStartListenerIndex = 0;
	static constexpr unsigned int animationShootingEndListenerIndex = 1;



	static constexpr unsigned int controllerLaserExpansionIndex = 0;
	static constexpr unsigned int controllerWalkingSpeedChangeIndex = 1;
	static constexpr unsigned int controllerInvulnerabilityTimerIndex = 2; /////////////////////////////////////

	static constexpr unsigned int rigidObjectListenerLaserIndex = 0;
	static constexpr unsigned int rigidObjectListenerPlayerIndex = 1; /////////////////////////////////////


	static constexpr short dirNone = 0;
	static constexpr short dirLeft = -1;
	static constexpr short dirRight = 1;

	static constexpr unsigned int particlesOnLaser = 100;


	// state

	bool shooting;
	short direction;
	bool walking;
	bool invulnerable;

	float acceleration; // units/s^2
	float maximumSpeed; // units/s
	float invulnerabilityTimeLeft; // s

	float laserExtensionSpeed; // units/s^2


	//callbacks and controllers

	std::array<std::unique_ptr<TouchArea::Listener>, 3> touchControlListeners;
	std::array<std::unique_ptr<AnimatedSprite::Listener>, 2> animationListeners;
	std::array<ControllerPtr, 3> controllers;
	std::array<bool, 3> controllersState;
	std::array<std::unique_ptr<RigidObject::Listener>, 2> rigidObjectListeners;

	Listener *enemyHitListener;


	// visuals

	SceneManager *scene;
	SceneNode *playerNode;

	AnimatedSprite *bodySprite;
	RigidObjectUPtr playerBounds;

	AnimatedSprite *laserHead;
	SingleSprite *laser;
	RigidObjectUPtr laserBounds;

	Vector2 laserPositionOffset; //offset from bodysprite

	ParticleSystem *laserParticleSystem;


public:

	// state management

	void create(InputManager *_inputManager, SceneManager *_scene, ScriptNodePtr _data);
	void setupCallbacks(InputManager *_inputManager);

	void registerEnemyHitListener(Listener *_enemyHitListener);
	void resetPlayer();

	void pause();
	void resume();


	// callback functions

	void updateDirection(short _direction, bool _addDirection); //add/remove direction
	void updateWalkingSpeed(float _timeDelta); // update movement speed
	void shootLaser(); //shoot laser
	void startLaser(); //display laser, enable physics bounds and register laser time controller
	void expandLaser(float _timeDelta); //expand laser (called by time controller)
	void endShootingLaser(); //laser hit wall, run end enimation and hide laser
	void shootingEnded(); //animation completed, enable movement
	void hitByEnemy(); //lost a live if not invulnerable
	void beginInvulnerability(float _time); //start invulnerability period
	void progressInvulnerability(float _timeDelta); //progres/end invulnerability


	// callback listeners etc

	class PlayerMovementInputControl : public TouchArea::Listener
	{
	protected:
		Player *owner;
		short direction;
	public:
		PlayerMovementInputControl(Player *_owner, short _direction);
		virtual ~PlayerMovementInputControl();
		void onPointerOnArea();
		void onPointerOffArea();
	};

	class PlayerShootingInputControl : public TouchArea::Listener
	{
	protected:
		Player *owner;
	public:
		PlayerShootingInputControl(Player *_owner);
		virtual ~PlayerShootingInputControl();
		void onPointerOnArea();
		void onPointerOffArea();
	};


	class LaserStartAnimationListener : public AnimatedSprite::Listener
	{
	protected:
		Player *owner;
	public:
		LaserStartAnimationListener(Player *_owner);
		virtual ~LaserStartAnimationListener();
		void onAnimationBegin();
		void onAnimationEnd();
	};

	class PlayerShootingEndAnimationListener : public AnimatedSprite::Listener
	{
	protected:
		Player *owner;
	public:
		PlayerShootingEndAnimationListener(Player *_owner);
		virtual ~PlayerShootingEndAnimationListener();
		void onAnimationBegin();
		void onAnimationEnd();
	};


	class LaserExpansionController : public ControllerValue
	{
	protected:
		Player *owner;
	public:
		LaserExpansionController(Player *_owner);
		virtual ~LaserExpansionController();
		float get() const;
		void set(float _value);
	};

	class WalkingSpeedChangeController : public ControllerValue
	{
	protected:
		Player *owner;
	public:
		WalkingSpeedChangeController(Player *_owner);
		virtual ~WalkingSpeedChangeController();
		float get() const;
		void set(float _value);
	};

	class InvulnerabilityTimerController : public ControllerValue
	{
	protected:
		Player *owner;
	public:
		InvulnerabilityTimerController(Player *_owner);
		virtual ~InvulnerabilityTimerController();
		float get() const;
		void set(float _value);
	};


	class LaserCollisionListener : public RigidObject::Listener
	{
	protected:
		Player *owner;
	public:
		LaserCollisionListener(Player *_owner);
		virtual ~LaserCollisionListener();
		void onCollisionDetected(RigidObject *_object);
		void onCollisionEnded(RigidObject *_object);
	};

	class PlayerCollisionListener : public RigidObject::Listener
	{
	protected:
		Player *owner;
	public:
		PlayerCollisionListener(Player *_owner);
		virtual ~PlayerCollisionListener();
		void onCollisionDetected(RigidObject *_object);
		void onCollisionEnded(RigidObject *_object);
	};


};

