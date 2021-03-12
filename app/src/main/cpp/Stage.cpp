#include "Stage.h"



void Stage::create(SceneManager *_scene, ScriptNodePtr _data)
{
	assert(_scene && "no scene manager provided");

	scene = _scene;
	backgroundNode = scene->getNodeByName(stageSceneNodeName);
	assert(backgroundNode && "background node not created");

	width = std::stof(_data->getValue(stageWidthValueName));
	height = std::stof(_data->getValue(stageHeightValueName));
	gravityStrength = std::stof(_data->getValue(stageGravityValueName));

	ScriptNodePtr bgSpriteNode;
	ScriptNodeListPtr nodeList = _data->getChildList();
	for (auto it = nodeList->begin(), itEnd = nodeList->end(); it != itEnd; ++it)
	{
		if ((*it)->getName().compare(bgSpriteNodeName) == 0)
		{
			bgSpriteNode = (*it);
			break;
		}
	}

	assert(bgSpriteNode && "background node not created");

	isAnimated = parse<bool>(bgSpriteNode->getValue(bgSpriteIsAnimatedValueName));

	if (!isAnimated)
	{
		staticBackground = scene->createSingleSprite(bgSpriteNodeName, bgSpriteNode);
		backgroundNode->appendObject(staticBackground);
	}
	else
	{
		animatedBackground = scene->createAnimatedSprite(bgSpriteNodeName, bgSpriteNode);
		backgroundNode->appendObject(animatedBackground);
	}


	PhysicsAffectorPtr gravity = std::make_unique<PhysicsLinearAffector>(Vector2(0.0f, -gravityStrength));
	PhysicsSystem::getSingletonPtr()->registerAffector(gravityAffector, gravity);


	bounds = std::make_unique<RigidObject>(rigidTypeStage, PhysicsSystem::getSingletonPtr());

	ShapePtr border;
	border = std::make_unique<LineArea>(Vector2(width * -0.5f, 1.0f), Vector2(width * -0.5f, -1.0f)); //left wall
	bounds->addShape(border);

	border = std::make_unique<LineArea>(Vector2(-1.0f, height * -0.5f), Vector2(1.0f, height * -0.5f)); //bottom wall
	bounds->addShape(border);

	border = std::make_unique<LineArea>(Vector2(width * 0.5f, -1.0f), Vector2(width * 0.5f, 1.0f)); //right wall
	bounds->addShape(border);


	bounds->setEnabled(false);
	bounds->setEntity(isAnimated ? static_cast<MovableObject*>(animatedBackground) : static_cast<MovableObject*>(staticBackground));

	setupCallbacks();

	// run default animation loop
	if(isAnimated)
		animatedBackground->playAnimation(spriteIdleAnimationValue, Animator::AM_REPEAT, false);
};


void Stage::pause()
{
	if(isAnimated)
		animatedBackground->pauseAnimation(true);

	bounds->setEnabled(false);
};


void Stage::resume()
{
	if(isAnimated)
		animatedBackground->pauseAnimation(false);

	bounds->setEnabled(true);
};


void Stage::playExplosion(const Vector2 &_position, float radius)
{
	ParticleEmitter *emiter = new InstanceParticleEmitters(explosionSystem, explosionParticleCount, {2.0f * radius, 2.0f * radius, _position, 0.0f});
	explosionSystem->addEmiter(emiter);
};
