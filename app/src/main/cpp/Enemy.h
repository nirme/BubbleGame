#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

#include "core/Logger.h"
#include "core/_2d/SceneManager.h"

#include "core/_2d/PhysicsSystem.h"
#include "core/_2d/PhysicsAffectors.h"

#include "core/_2d/RigidObject.h"
#include "core/_2d/shapes/ShapeFactory.h"


#include "GameStrings.h"
#include "Stage.h"


using namespace core;
using namespace core::_2d;


class Enemy
{
public:

	enum LEVEL : unsigned short
	{
		LEVEL_1 = 0,
		LEVEL_2 = 1,
		LEVEL_3 = 2,
		LAST_LEVEL = 2,
		UNDEFINED = 0xFF
	};

	class Listener
	{
	public:
		virtual void onLaserHit(Enemy *_subject) = 0;
		virtual ~Listener(){};
	};

protected:

	static constexpr char *enemySceneNodeName = "enemy_node";

	static constexpr char *enemyLevelValueName = "enemy_level";

	static constexpr char *enemySpriteNodeName = "sprite";
	static constexpr char *enemyBoundsNodeName = "bounds";

	static constexpr float invulnerabilityFlickerTime  = 0.5f;
	static constexpr float invulnerabilityTimeOnSpawn  = 2.0f; // in seconds

	static constexpr char *enemySpriteName = "enemy_sprite_%d";
	static int enemyCounter;

	static std::string getNextName();

	//callbacks and controllers

	std::unique_ptr<RigidObject::Listener> rigidObjectListener;
	ControllerPtr timeoutController;
	bool timeoutControllerState;
	std::unique_ptr<Listener> listener;
	Stage *stage;



	bool alive;
	bool invulnerable;
	float invulnerabilityTimeLeft; // s

	LEVEL enemyLevel;

	SceneManager *scene;
	SceneNode *enemyNode;

	SingleSprite *sprite;
	RigidObjectUPtr bounds;



public:

	Enemy();
	Enemy(const Enemy &_rhs);

	void create(SceneManager *_scene, ScriptNodePtr _data, Stage *_stage);

	void setupCallbacks();
	void registerListener(std::unique_ptr<Listener> _listener);

	void setLevel(LEVEL _level);
	LEVEL getLevel() const;
	void lowerLevel();
	void kill();

	Vector2 getPosition() const;
	Vector2 getDirection() const;
	float getRadius() const;

	void pause();
	void resume();

	// callback functions

	void hitByLaser(); // call listener and kill instance
	void beginInvulnerability(float _time); //start invulnerability period
	void progressInvulnerability(float _timeDelta);
	void spawn(const Vector2 &_position, const Vector2 &_direction);


	// callback listeners etc

	class CollisionListener : public RigidObject::Listener
	{
	protected:
		Enemy *owner;
	public:
		CollisionListener(Enemy *_owner);
		virtual ~CollisionListener();
		void onCollisionDetected(RigidObject *_object);
		void onCollisionEnded(RigidObject *_object);
	};



	class InvulnerabilityTimerController : public ControllerValue
	{
	protected:
		Enemy *owner;
	public:
		InvulnerabilityTimerController(Enemy *_owner);
		virtual ~InvulnerabilityTimerController();
		float get() const;
		void set(float _value);
	};

};

