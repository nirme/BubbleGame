#include "GameObjects.h"




















void Stage::resetStage()
{
	width = 0.0f;
	height = 0.0f;
	gravityStrength = 1.0f;

	if (backgroundNode)
	{
		scene->destroyNode(backgroundNode);
		backgroundNode = nullptr;
		baseObject = nullptr;
	}

	bounds.reset();
};


void Stage::setBackground(ScriptNodePtr _bgData)
{
	assert(scene);

	if (bounds)
		bounds->setEntity(nullptr);

	if (backgroundNode)
	{
		scene->destroyNode(backgroundNode);
		backgroundNode = nullptr;
		baseObject = nullptr;
	}

	backgroundNode = scene->createNode(_bgData);

	ScriptNodeListPtr objectList = _bgData->getChildList();
	for (ScriptNodeListIterator it = objectList->begin(); it != objectList->end(); ++it)
	{
		if ((*it)->getName().compare("object") == 0)
		{
			MovableObject *object = scene->createObject(*it);
			if (object)
				backgroundNode->appendObject(object);
		}
	}

	baseObject = backgroundNode->getObjectByName(stageObjectName);
	if (!baseObject)
	{
		SceneNode::ObjectIterator it = backgroundNode->getObjectIterator();
		baseObject = !it.isEnd() ? *it : nullptr;
	}

	assert(baseObject && "No object to attach physical bounds to");

	if (bounds)
		bounds->setEntity(baseObject);
};


void Stage::setGravity(float _gravityStrength)
{
	PhysicsLinearAffector *gravityAffector = nullptr;
	PhysicsAffectorPtr gravity = PhysicsSystem::getSingleton().getAffector(gravityPhysAffectorName);
	if (!gravity)
	{
		PhysicsSystem::getSingleton().registerAffector(
				gravityPhysAffectorName,
				std::make_shared<PhysicsLinearAffector>({0.0f, -_gravityStrength})
		);

		gravityStrength = _gravityStrength;
	}
	else if (_gravityStrength != gravityStrength)
	{
		PhysicsLinearAffector *gravityAffector = dynamic_cast<PhysicsLinearAffector*>(gravity.get());
		assert(gravityAffector && "gravity affector have incorrect type");

		gravityAffector->setDirection({0.0f, -_gravityStrength});

		gravityStrength = _gravityStrength;
	}
};


void Stage::setupBounds(float _width, float _height)
{
	assert(_width > 0.0f && _height > 0.0f && "width and height must be bigger than 0");

	if (!bounds)
	{
		bounds = std::make_unique<RigidObject>(rigidTypeStage, nullptr);

		if (baseObject)
			bounds->setEntity(baseObject);

		bounds->setBounceFactor(1.0f);
		bounds->setStatic(true);
		bounds->setEnabled(true);
		bounds->addAffector(gravityPhysAffectorName);
		PhysicsSystem::getSingleton().registerObject(bounds.get());
	}
	else if (baseObject)
		bounds->setEntity(baseObject);

	if (_width == width && _height == height)
		return;

	width = _width;
	height = _height;


	bounds->clearShapes();

	// add updated shapes
	{
		ShapePtr leftWall(new LineArea({(width * -0.5f), 1.0f}, {(width * -0.5f), -1.0f}));
		ShapePtr rightWall(new LineArea({(width * 0.5f), -1.0f}, {(width * 0.5f), 1.0f}));
		ShapePtr floor(new LineArea({-1.0f, (height * -0.5f)}, {1.0f, (height * -0.5f)}));
		ShapePtr ceiling(new LineArea({1.0f, (height * 0.5f)}, {-1.0f, (height * 0.5f)}));

		bounds->addShape(leftWall);
		bounds->addShape(rightWall);
		bounds->addShape(floor);
		bounds->addShape(ceiling);
	}
};
