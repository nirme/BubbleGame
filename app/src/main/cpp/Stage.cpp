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
};


void Stage::playAnimation()
{
	if(isAnimated)
	{
		if (animatedBackground->isPaused())
			animatedBackground->pauseAnimation(false);
		else
			animatedBackground->playAnimation(spriteIdleAnimationValue, Animator::AM_REPEAT, false);
	}
};


void Stage::pauseAnimation()
{
	if(isAnimated)
	{
		animatedBackground->pauseAnimation(true);
	}
};

