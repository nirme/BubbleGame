#pragma once

#include <memory>
#include <string>
#include <vector>


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

#include "core/InputManager.h"
#include "core/TouchControls.h"

#include "core/StringUtils.h"


using namespace core;
using namespace core::_2d;

static constexpr char *rigidTypePlayer = "player";
static constexpr char *rigidTypeLaser = "laser";
static constexpr char *rigidTypeStage = "stage";
static constexpr char *rigidTypeEnemy = "enemy";


class Player
{
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
	static constexpr float invulnerabilityTimeAfterEnemyHit = 3.0f;
	static constexpr float invulnerabilityFlickerTime  = 0.5f;


	static constexpr char *animationPlayerIdle = "idle";
	static constexpr char *animationPlayerWalk = "walk";
	static constexpr char *animationLaserHeadShooting = "fire_loop";
	//static constexpr char *animationLaserHeadAgaintsWall = "wall_hit_loop";


	static constexpr char *inputLeftControlName = "touch_left";
	static constexpr char *inputRightControlName = "touch_right";
	static constexpr char *inputCenterControlName = "touch_center";

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

	std::unique_ptr<TouchArea::Listener> touchControlListeners[3];
	std::unique_ptr<AnimatedSprite::Listener> animationListeners[2];
	ControllerPtr controllers[3];
	std::unique_ptr<RigidObject::Listener> rigidObjectListeners[2];


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








	void create(SceneManager *_scene, ScriptNodePtr _data)
	{
		// move affector to load from script
		if (!PhysicsSystem::getSingleton().getAffector(movementAffector))
		{
			PhysicsAffectorPtr playerAffector = std::make_unique<PhysicsSpeedAffector>(movementAffectorOperation, movementAffectorFactor);
			PhysicsSystem::getSingleton().registerAffector(movementAffector, playerAffector);
		}

		shooting = false;
		direction = dirNone;
		walking = false;
		invulnerable = false;
		invulnerabilityTimeLeft = 0.0f;


		acceleration = std::stof(_data->getValue(accelerationValueName));
		maximumSpeed = std::stof(_data->getValue(maximumSpeedValueName));
		laserExtensionSpeed = std::stof(_data->getValue(laserExtensionSpeedValueName));
		laserPositionOffset = stov2(_data->getValue(laserPositionOffsetValueName));


		scene = _scene;
		ScriptNodeListPtr nodeList = _data->getChildList();
		ScriptNodeList::iterator res = std::find_if((*nodeList).begin(),
													(*nodeList).end(),
													[] (ScriptNodePtr _node) { return !_node->getName().compare(playerSceneNodeName); });

		assert(res != nodeList->end() && "player scene node data not found");
		ScriptNodePtr sceneNodeData = *res;

		playerNode = scene->createNode(sceneNodeData);

// load visuals from data provided

/*
AnimatedSprite *bodySprite;
RigidObjectUPtr playerBounds;

AnimatedSprite *laserHead;
SingleSprite *laser;
RigidObjectUPtr laserBounds;

ParticleSystem *laserParticleSystem;








		static constexpr char *bodySpriteNodeName = "body_sprite";
		static constexpr char *laserHeadNodeName = "laser_head";
		static constexpr char *laserNodeName = "laser";
		static constexpr char *laserParticleSystemNodeName = "laser_particle_system";

		static constexpr char *playerBoundsNodeName = "player_bounds";
		static constexpr char *laserBoundsNodeName = "laser_bounds";
*/
	};



	void setupCallbacks(InputManager *_inputManager)
	{
		// input callbacks
		TouchControl *touchControl(nullptr);

		// left button
		touchControl = _inputManager->getControlByName(inputLeftControlName);
		assert(touchControl && "touch control not found");
		if (!touchControlListeners[inputLeftControlIndex])
			touchControlListeners[inputLeftControlIndex] = std::make_unique<PlayerMovementInputControl>(this, DirLeft);
		touchControl->registerListener(touchControlListeners[inputLeftControlIndex].get());

		// right button
		touchControl = _inputManager->getControlByName(inputRightControlName);
		assert(touchControl && "touch control not found");
		if (!touchControlListeners[inputRightControlIndex])
			touchControlListeners[inputRightControlIndex] = std::make_unique<PlayerMovementInputControl>(this, DirRight);
		touchControl->registerListener(touchControlListeners[inputRightControlIndex].get());

		// shooting button
		touchControl = _inputManager->getControlByName(inputCenterControlName);
		assert(touchControl && "touch control not found");
		if (!touchControlListeners[inputShootingControlIndex])
			touchControlListeners[inputShootingControlIndex] = std::make_unique<PlayerShootingInputControl>(this);
		touchControl->registerListener(touchControlListeners[inputShootingControlIndex].get());


		// start laser when in position
		if (!animationListeners[animationShootingStartListenerIndex])
			animationListeners[animationShootingStartListenerIndex] = std::make_unique<LaserStartAnimationListener>(this);
		bodySprite->registerAnimationListener(animationShootingStart, animationListeners[animationShootingStartListenerIndex].get());

		// end sequence when animation reaches end
		if (!animationListeners[animationShootingEndListenerIndex])
			animationListeners[animationShootingEndListenerIndex] = std::make_unique<PlayerShootingEndAnimationListener>(this);
		bodySprite->registerAnimationListener(animationShootingEnd, animationListeners[animationShootingEndListenerIndex].get());


		// create laser expansion controller value
		if (!controllers[controllerLaserExpansionIndex])
		{
			SharedControllerValuePtr laserExpansionControllerValue = std::make_shared<LaserExpansionController>(this);
			controllers[controllerLaserExpansionIndex] = ControllerManager::getSingleton().createFrameTimeController(laserExpansionControllerValue);
			controllers[controllerLaserExpansionIndex]->setEnabled(false);
		}

		// create walking speed controller value and register for updates
		if (!controllers[controllerWalkingSpeedChangeIndex])
		{
			SharedControllerValuePtr walkingControllerValue = std::make_shared<WalkingSpeedChangeController>(this);
			controllers[controllerWalkingSpeedChangeIndex] = ControllerManager::getSingleton().createFrameTimeController(walkingControllerValue);
			controllers[controllerWalkingSpeedChangeIndex]->setEnabled(false);
		}

		// create invulnerability controller value and register for updates
		if (!controllers[controllerInvulnerabilityTimerIndex])
		{
			SharedControllerValuePtr invulnerabilityControllerValue = std::make_shared<InvulnerabilityTimerController>(this);
			controllers[controllerInvulnerabilityTimerIndex] = ControllerManager::getSingleton().createFrameTimeController(invulnerabilityControllerValue);
			controllers[controllerInvulnerabilityTimerIndex]->setEnabled(false);
		}


		// register laser collision
		if (!rigidObjectListeners[rigidObjectListenerLaserIndex])
			rigidObjectListeners[rigidObjectListenerLaserIndex] = std::make_unique<LaserCollisionListener>(this);
		laserBounds->registerListener(rigidObjectListeners[rigidObjectListenerLaserIndex].get());

		// register player collision
		if (!rigidObjectListeners[rigidObjectListenerPlayerIndex])
			rigidObjectListeners[rigidObjectListenerPlayerIndex] = std::make_unique<PlayerCollisionListener>(this);
		laserBounds->registerListener(rigidObjectListeners[rigidObjectListenerPlayerIndex].get());
	};



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

