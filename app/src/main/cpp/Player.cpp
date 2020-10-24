#include "Player.h"



void Player::create(InputManager *_inputManager, SceneManager *_scene, ScriptNodePtr _data)
{
	assert(_scene && "no scene manager provided");

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


	nodeList = sceneNodeData->getChildList();

	for (auto it = nodeList->begin(), itEnd = nodeList->end(); it != itEnd; ++it)
	{
		if ((*it)->getName().compare(bodySpriteNodeName) == 0)
		{
			bodySprite = scene->createAnimatedSprite(bodySpriteNodeName, *it);
			playerNode->appendObject(bodySprite);
		}
		else if ((*it)->getName().compare(laserHeadNodeName) == 0)
		{
			laserHead = scene->createAnimatedSprite(laserHeadNodeName, *it);
			playerNode->appendObject(laserHead);
		}
		else if ((*it)->getName().compare(laserNodeName) == 0)
		{
			laser = scene->createSingleSprite(laserNodeName, (*it));
			playerNode->appendObject(laser);
		}
		else if ((*it)->getName().compare(laserParticleSystemNodeName) == 0)
		{
			laserParticleSystem = scene->createParticleSystem(laserParticleSystemNodeName, (*it));
			playerNode->appendObject(laserParticleSystem);
		}
		else if ((*it)->getName().compare(playerBoundsNodeName) == 0)
		{
			playerBounds = std::make_unique<RigidObject>(rigidTypePlayer, PhysicsSystem::getSingletonPtr());
			playerBounds->addAffector(movementAffector);

			ScriptNodeListPtr shapeList = (*it)->getChildList();
			for (auto shapeIt = shapeList->begin(), shapeItEnd = shapeList->end(); shapeIt != shapeItEnd; ++shapeIt)
			{
				if ((*shapeIt)->getName().compare("shape") == 0)
				{
					ShapePtr parsedShape = ShapeFactory::parseShape(*shapeIt);
					playerBounds->addShape(parsedShape);
				}
			}
		}
		else if ((*it)->getName().compare(laserBoundsNodeName) == 0)
		{
			laserBounds = std::make_unique<RigidObject>(rigidTypePlayer, PhysicsSystem::getSingletonPtr());

			ScriptNodeListPtr shapeList = (*it)->getChildList();
			for (auto shapeIt = shapeList->begin(), shapeItEnd = shapeList->end(); shapeIt != shapeItEnd; ++shapeIt)
			{
				if ((*shapeIt)->getName().compare("shape") == 0)
				{
					ShapePtr parsedShape = ShapeFactory::parseShape(*shapeIt);
					laserBounds->addShape(parsedShape);
				}
			}
		}
	}

	playerBounds->setEntity(bodySprite);
	laserBounds->setEntity(laser);

	setupCallbacks(_inputManager);
};


void Player::setupCallbacks(InputManager *_inputManager)
{
	// input callbacks
	TouchControl *touchControl(nullptr);

	// left button
	touchControl = _inputManager->getControlByName(inputLeftControlName);
	assert(touchControl && "touch control not found");
	if (!touchControlListeners[inputLeftControlIndex])
		touchControlListeners[inputLeftControlIndex] = std::make_unique<PlayerMovementInputControl>(this, dirLeft);
	touchControl->registerListener(touchControlListeners[inputLeftControlIndex].get());

	// right button
	touchControl = _inputManager->getControlByName(inputRightControlName);
	assert(touchControl && "touch control not found");
	if (!touchControlListeners[inputRightControlIndex])
		touchControlListeners[inputRightControlIndex] = std::make_unique<PlayerMovementInputControl>(this, dirRight);
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


void Player::registerEnemyHitListener(Listener *_enemyHitListener)
{
	enemyHitListener = _enemyHitListener;
};


void Player::resetPlayer()
{
	unsigned int i=0;
	while (i < controllers.size())
		controllers[i++]->setEnabled(false);

	shooting = false;
	direction = dirNone;
	walking = false;
	invulnerable = false;
	invulnerabilityTimeLeft = 0.0f;

	playerBounds->setDirectionVector({0.0f});
	laserBounds->setDirectionVector({0.0f});
	laserBounds->setEnabled(false);

	bodySprite->stopAnimation();
	bodySprite->setEnabled(true);

	laserHead->stopAnimation();
	laserHead->setEnabled(false);

	laser->setEnabled(false);

	laserParticleSystem->reset();
};


void Player::updateDirection(short _direction, bool _addDirection) //add/remove direction
{
	Logger::getSingleton().debug(__FUNCTION__);

	if (_addDirection)
		direction += _direction;
	else
		direction -= _direction;

	if (shooting)
		return;

	if (!direction && walking)
	{
		bodySprite->stopAnimation();
		bodySprite->playAnimation(animationPlayerIdle, Animator::AM_REPEAT);
		controllers[controllerWalkingSpeedChangeIndex]->setEnabled(false);
		walking = false;
	}
	else if (direction)
	{
		Vector2 scale = bodySprite->getScale();
		scale.x = direction > 0 ? std::abs(scale.x) : -std::abs(scale.x);
		bodySprite->setScale(scale);
		if (!walking)
		{
			bodySprite->playAnimation(animationPlayerWalk, Animator::AM_REPEAT);
			controllers[controllerWalkingSpeedChangeIndex]->setEnabled(true);
			walking = true;
		}
	}
};


void Player::updateWalkingSpeed(float _timeDelta) // update movement speed
{
	Logger::getSingleton().debug(__FUNCTION__);

	if (shooting || !walking || !direction)
		return;

	float speedChange = direction * (acceleration * _timeDelta);
	Vector2 speedVector = playerBounds->getDirectionVector();

	speedVector.x += speedChange;
	if (std::abs(speedVector.x) > maximumSpeed)
		speedVector.x = maximumSpeed * direction;

	playerBounds->setDirectionVector(speedVector);
};


void Player::shootLaser() //shoot laser
{
	Logger::getSingleton().debug(__FUNCTION__);

	if (shooting)
		return;

	controllers[controllerWalkingSpeedChangeIndex]->setEnabled(false);
	//bodySprite->stopAnimation();
	bodySprite->playAnimation(animationShootingStart, Animator::AM_ONCE, true);
	// end animation listener will call startLaser() when done
};


void Player::startLaser() //display laser, enable physics bounds and register laser time controller
{
	Logger::getSingleton().debug(__FUNCTION__);

	Vector2 newLaserPosition = bodySprite->getPosition() + laserPositionOffset;
	laser->setPosition(newLaserPosition);

	newLaserPosition.y -= laser->getScale().y;
	laserHead->setPosition(newLaserPosition);

	laser->setEnabled(true);
	laserBounds->setEnabled(true);
	laserHead->setEnabled(true);

	laserHead->playAnimation(animationLaserHeadShooting, Animator::AM_REPEAT);
	controllers[controllerLaserExpansionIndex]->setEnabled(true);
};


void Player::expandLaser(float _timeDelta) //expand laser (called by time controller)
{
	Logger::getSingleton().debug(__FUNCTION__);

	// in case main body body position changed
	Vector2 laserScale = laser->getScale();
	laserScale.y += laserExtensionSpeed * _timeDelta;
	laser->setScale(laserScale);

	Vector2 newLaserPosition = bodySprite->getPosition() + laserPositionOffset;
	laser->setPosition(newLaserPosition);

	newLaserPosition.y -= laserScale.y;
	laserHead->setPosition(newLaserPosition);
};


void Player::endShootingLaser() //laser hit wall, run end enimation and hide laser
{
	Logger::getSingleton().debug(__FUNCTION__);

	bodySprite->playAnimation(animationShootingEnd, Animator::AM_ONCE, true);

	Vector2 laserSize = laser->getSpriteCoords().uvPoints[1] - laser->getSpriteCoords().uvPoints[2];
	laserSize.x *= laser->getScale().x;
	laserSize.y *= laser->getScale().y;

	ParticleEmitter* emitter = new InstanceParticleEmitters(laserParticleSystem, particlesOnLaser, Rectangle(laserSize.x, laserSize.y));
	emitter->position = laser->getPosition() + (laserSize * 0.5f);
	emitter->setEmissionSizeChangeSpeed(1.1f, 1.5f);
	emitter->setEmissionTimeAlive(1.0f, 2.0f);
	laserParticleSystem->addEmiter(emitter);

	controllers[controllerLaserExpansionIndex]->setEnabled(false);
	laserHead->stopAnimation();
	laserBounds->setEnabled(false);

	laser->setEnabled(false);
	laserHead->setEnabled(false);
};


void Player::shootingEnded() //animation completed, enable movement
{
	Logger::getSingleton().debug(__FUNCTION__);

	if (!direction)
	{
		bodySprite->playAnimation(animationPlayerIdle, Animator::AM_REPEAT);
		controllers[controllerWalkingSpeedChangeIndex]->setEnabled(false);
		walking = false;
	}
	else
	{
		bodySprite->playAnimation(animationPlayerWalk, Animator::AM_REPEAT);
		controllers[controllerWalkingSpeedChangeIndex]->setEnabled(true);
		walking = true;
	}

};


void Player::hitByEnemy()
{
	if (!invulnerable)
	{
		if (enemyHitListener)
			enemyHitListener->onEnemyHit();
		beginInvulnerability(invulnerabilityTimeAfterEnemyHit);
	}
};


void Player::beginInvulnerability(float _time)
{
	invulnerable = true;
	invulnerabilityTimeLeft = _time;
	controllers[controllerInvulnerabilityTimerIndex]->setEnabled(true);
};


void Player::progressInvulnerability(float _timeDelta)
{
	if (invulnerabilityTimeLeft > _timeDelta)
	{
		invulnerabilityTimeLeft -= _timeDelta;
		bool newState = ((int) (invulnerabilityTimeLeft / invulnerabilityFlickerTime)) % 2 != 0;
		if (bodySprite->isEnabled() != newState)
			bodySprite->setEnabled(newState);
	}
	else
	{
		controllers[controllerInvulnerabilityTimerIndex]->setEnabled(false);
		invulnerabilityTimeLeft = 0.0f;
		invulnerable = false;
		bodySprite->setEnabled(true); // might be hidden
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////


// PlayerMovementInputControl

Player::PlayerMovementInputControl::PlayerMovementInputControl(Player *_owner, short _direction) :
		TouchArea::Listener(),
		owner(_owner),
		direction(_direction)
{};

Player::PlayerMovementInputControl::~PlayerMovementInputControl()
{};

void Player::PlayerMovementInputControl::onPointerOnArea()
{
	owner->updateDirection(direction, true); /*add to direction*/
};

void Player::PlayerMovementInputControl::onPointerOffArea()
{
	owner->updateDirection(direction, false); /*remove from direction*/
};


// PlayerShootingInputControl

Player::PlayerShootingInputControl::PlayerShootingInputControl(Player *_owner) :
		TouchArea::Listener(),
		owner(_owner)
{};

Player::PlayerShootingInputControl::~PlayerShootingInputControl()
{};

void Player::PlayerShootingInputControl::onPointerOnArea()
{
	owner->shootLaser();
};

void Player::PlayerShootingInputControl::onPointerOffArea()
{};


// LaserStartAnimationListener

Player::LaserStartAnimationListener::LaserStartAnimationListener(Player *_owner) :
		AnimatedSprite::Listener(),
		owner(_owner)
{};

Player::LaserStartAnimationListener::~LaserStartAnimationListener()
{};

void Player::LaserStartAnimationListener::onAnimationBegin()
{};

void Player::LaserStartAnimationListener::onAnimationEnd()
{
	owner->startLaser(); /*display laser and start expanding it*/
};


// PlayerShootingEndAnimationListener

Player::PlayerShootingEndAnimationListener::PlayerShootingEndAnimationListener(Player *_owner) :
		AnimatedSprite::Listener(),
		owner(_owner)
{};

Player::PlayerShootingEndAnimationListener::~PlayerShootingEndAnimationListener()
{};

void Player::PlayerShootingEndAnimationListener::onAnimationBegin()
{};

void Player::PlayerShootingEndAnimationListener::onAnimationEnd()
{
	owner->shootingEnded(); /*display laser and start expanding it*/
};


// LaserExpansionController

Player::LaserExpansionController::LaserExpansionController(Player *_owner) :
		ControllerValue(),
		owner(_owner)
{};

Player::LaserExpansionController::~LaserExpansionController()
{};

float Player::LaserExpansionController::get() const
{
	return 0.0f;
};

void Player::LaserExpansionController::set(float _value)
{
	owner->expandLaser(_value);
};


// WalkingSpeedChangeController

Player::WalkingSpeedChangeController::WalkingSpeedChangeController(Player *_owner) :
		ControllerValue(),
		owner(_owner)
{};

Player::WalkingSpeedChangeController::~WalkingSpeedChangeController()
{};

float Player::WalkingSpeedChangeController::get() const
{
	return 0.0f;
};

void Player::WalkingSpeedChangeController::set(float _value)
{
	owner->updateWalkingSpeed(_value);
};


// InvulnerabilityTimerController

Player::InvulnerabilityTimerController::InvulnerabilityTimerController(Player *_owner) :
		ControllerValue(),
		owner(_owner)
{};

Player::InvulnerabilityTimerController::~InvulnerabilityTimerController()
{};

float Player::InvulnerabilityTimerController::get() const
{
	return 0.0f;
};

void Player::InvulnerabilityTimerController::set(float _value)
{
	owner->progressInvulnerability(_value);
};


// LaserCollisionListener

Player::LaserCollisionListener::LaserCollisionListener(Player *_owner) :
		RigidObject::Listener(),
		owner(_owner)
{};

Player::LaserCollisionListener::~LaserCollisionListener()
{};

void Player::LaserCollisionListener::onCollisionDetected(RigidObject *_object)
{
	if (_object->getCollidableObjectType().compare(rigidTypeStage) == 0)
		owner->endShootingLaser();
};

void Player::LaserCollisionListener::onCollisionEnded(RigidObject *_object)
{};


// PlayerCollisionListener

Player::PlayerCollisionListener::PlayerCollisionListener(Player *_owner) :
		RigidObject::Listener(),
		owner(_owner)
{};

Player::PlayerCollisionListener::~PlayerCollisionListener()
{};

void Player::PlayerCollisionListener::onCollisionDetected(RigidObject *_object)
{
	if (_object->getCollidableObjectType().compare(rigidTypeEnemy) == 0)
		owner->hitByEnemy();
};

void Player::PlayerCollisionListener::onCollisionEnded(RigidObject *_object)
{};




