#include "Player.h"



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
		--lives;
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
{};

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
{};

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
{};

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




