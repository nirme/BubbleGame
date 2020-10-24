#include "Enemy.h"



void Enemy::create(SceneManager *_scene, ScriptNodePtr _data)
{
	assert(_scene && "no scene manager provided");

	alive = false;
	invulnerable = false;
	invulnerabilityTimeLeft = 0.0f;
	enemyLevel = (LEVEL) std::stoi(_data->getValue(enemyLevelValueName));

	scene = _scene;
	enemyNode = scene->getNodeByName(enemySceneNodeName);
	assert(enemyNode && "enemy node not created");


	ScriptNodeListPtr nodeList = _data->getChildList();

	for (auto it = nodeList->begin(), itEnd = nodeList->end(); it != itEnd; ++it)
	{
		if ((*it)->getName().compare(enemySpriteNodeName) == 0)
		{
			sprite = scene->createSingleSprite(enemySpriteNodeName, *it);
			enemyNode->appendObject(sprite);
		}
		else if ((*it)->getName().compare(enemyBoundsNodeName) == 0)
		{
			bounds = std::make_unique<RigidObject>(rigidTypePlayer, PhysicsSystem::getSingletonPtr());

			ScriptNodeListPtr shapeList = (*it)->getChildList();
			for (auto shapeIt = shapeList->begin(), shapeItEnd = shapeList->end(); shapeIt != shapeItEnd; ++shapeIt)
			{
				if ((*shapeIt)->getName().compare("shape") == 0)
				{
					ShapePtr parsedShape = ShapeFactory::parseShape(*shapeIt);
					bounds->addShape(parsedShape);
				}
			}
		}
	}

	bounds->setEnabled(false);
	bounds->setEntity(sprite);
	bounds->addAffector(gravityAffector);

	setupCallbacks();
};


void Enemy::setupCallbacks()
{
	if (!rigidObjectListener)
	{
		rigidObjectListener = std::make_unique<CollisionListener>(this);
	}

	if (!timeoutController)
	{
		SharedControllerValuePtr invulnerabilityTimerControllerValue = std::make_shared<InvulnerabilityTimerController>(this);
		timeoutController = ControllerManager::getSingleton().createFrameTimeController(invulnerabilityTimerControllerValue);
		timeoutController->setEnabled(false);
	}
};


void Enemy::registerListener(Listener *_listener)
{
	listener = _listener;
};


void Enemy::hitByLaser()
{
	if (!invulnerable)
	{
		if (listener)
			listener->onLaserHit(this);

		alive = false;
		sprite->setEnabled(false);
		bounds->setEnabled(false);
	}
};


void Enemy::beginInvulnerability(float _time)
{
	invulnerable = true;
	invulnerabilityTimeLeft = _time;
	timeoutController->setEnabled(true);
};


void Enemy::progressInvulnerability(float _timeDelta)
{
	if (invulnerabilityTimeLeft > _timeDelta)
	{
		invulnerabilityTimeLeft -= _timeDelta;
		bool newState = ((int) (invulnerabilityTimeLeft / invulnerabilityFlickerTime)) % 2 != 0;
		if (sprite->isEnabled() != newState)
			sprite->setEnabled(newState);
	}
	else
	{
		timeoutController->setEnabled(false);
		invulnerabilityTimeLeft = 0.0f;
		invulnerable = false;
		sprite->setEnabled(true); // might be hidden
	}
};


void Enemy::spawn(const Vector2 &_position, const Vector2 &_direction)
{
	sprite->setPosition(_position);
	bounds->setDirectionVector(_direction);

	sprite->setEnabled(true);
	bounds->setEnabled(true);

	beginInvulnerability(invulnerabilityTimeOnSpawn);

	alive = true;
};


Enemy::CollisionListener::CollisionListener(Enemy *_owner) :
	RigidObject::Listener(),
	owner(_owner)
{};


Enemy::CollisionListener::~CollisionListener()
{};


void Enemy::CollisionListener::onCollisionDetected(RigidObject *_object)
{
	if (_object->getCollidableObjectType().compare(rigidTypeLaser) == 0)
		owner->hitByLaser();
};


void Enemy::CollisionListener::onCollisionEnded(RigidObject *_object)
{};


Enemy::InvulnerabilityTimerController::InvulnerabilityTimerController(Enemy *_owner) :
	ControllerValue(),
	owner(_owner)
{};


Enemy::InvulnerabilityTimerController::~InvulnerabilityTimerController()
{};


float Enemy::InvulnerabilityTimerController::get() const
{
	return 0.0f;
};


void Enemy::InvulnerabilityTimerController::set(float _value)
{
	owner->progressInvulnerability(_value);
};
