#include "Enemy.h"


int Enemy::enemyCounter = 1;

std::string Enemy::getNextName()
{
	char name[32];
	printf(name, enemySpriteName, enemyCounter++);
	return std::string(name);
};


Enemy::Enemy() :
		alive(false),
		invulnerable(false),
		invulnerabilityTimeLeft(0),
		enemyLevel(UNDEFINED),
		scene(nullptr),
		enemyNode(nullptr),
		sprite(nullptr),
		bounds(nullptr),
		stage(nullptr)
{};


Enemy::Enemy(const Enemy &_rhs) :
		alive(false),
		invulnerable(false),
		invulnerabilityTimeLeft(0.0f),
		enemyLevel(_rhs.enemyLevel),
		scene(_rhs.scene),
		enemyNode(_rhs.enemyNode),
		stage(_rhs.stage)
{
	if (!scene)
		return;

	sprite = dynamic_cast<SingleSprite*>(_rhs.sprite->clone(getNextName()));
	enemyNode->appendObject(sprite);

	bounds = std::make_unique<RigidObject>(*_rhs.bounds);

	bounds->setEnabled(false);
	bounds->setEntity(sprite);

	setupCallbacks();
};


void Enemy::create(SceneManager *_scene, ScriptNodePtr _data, Stage *_stage)
{
	assert(_scene && "no scene manager provided");

	alive = false;
	invulnerable = false;
	invulnerabilityTimeLeft = 0.0f;
	enemyLevel = (LEVEL) std::stoi(_data->getValue(enemyLevelValueName));
	stage = _stage;

	scene = _scene;
	enemyNode = scene->getNodeByName(enemySceneNodeName);
	assert(enemyNode && "enemy node not created");


	ScriptNodeListPtr nodeList = _data->getChildList();

	for (auto it = nodeList->begin(), itEnd = nodeList->end(); it != itEnd; ++it)
	{
		if ((*it)->getName().compare(enemySpriteNodeName) == 0)
		{
			sprite = scene->createSingleSprite(getNextName(), *it);
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


void Enemy::registerListener(std::unique_ptr<Listener> _listener)
{
	listener.reset();
	listener.swap(_listener);
};


void Enemy::setLevel(LEVEL _level)
{
	enemyLevel = _level;
};


Enemy::LEVEL Enemy::getLevel() const
{
	return enemyLevel;
};


void Enemy::lowerLevel()
{
	enemyLevel = (LEVEL)((int)enemyLevel + 1);
	sprite->setScale(sprite->getScale() * 0.5f);
};


void Enemy::kill()
{
	// add timer with scaling down to 0 instead of turning right off
	alive = false;
	sprite->setEnabled(false);

	stage->playExplosion(getPosition(), getRadius()); //show explosion from shared particle manager
};


Vector2 Enemy::getPosition() const
{
	return bounds->getCurrentPosition();
};


Vector2 Enemy::getDirection() const
{
	return bounds->getDirectionVector();
};


float Enemy::getRadius() const
{
	const AxisAlignedBox &aabb = bounds->getBoundingBox();
	return (aabb.getMaximum().x - aabb.getMinimum().x) * 0.5f;
};


void Enemy::pause()
{
	if (timeoutControllerState = timeoutController->isEnabled())
		timeoutController->setEnabled(false);

	bounds->setEnabled(false);
};


void Enemy::resume()
{
	if (timeoutControllerState)
		timeoutController->setEnabled(true);

	bounds->setEnabled(false);
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
