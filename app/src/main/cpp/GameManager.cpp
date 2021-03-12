#include "GameManager.h"






GameManager::GameControl::GameControl(GameManager *_parent, Function _memberFunc) :
	TouchControl::Listener(),
	parent(_parent),
	memberFunc(_memberFunc)
{};


void GameManager::GameControl::onButtonClicked()
{
	(parent->*memberFunc)();
};


GameManager::ActionMenuChange::ActionMenuChange(GameManager *_parent, bool _popFromStack, bool _pushToStack, std::string _newMenuName) :
	TouchControl::Listener(),
	parent(_parent),
	popFromStack(_popFromStack),
	pushToStack(_pushToStack),
	newMenuName(_newMenuName)
{};


void GameManager::ActionMenuChange::onButtonClicked()
{
	if (popFromStack)
		parent->popMenu();

	if (pushToStack)
		parent->pushMenu(newMenuName);
};


GameManager::EnemyListener::EnemyListener(GameManager *_manager) :
	Enemy::Listener(),
	manager(_manager)
{};


void GameManager::EnemyListener::onLaserHit(Enemy *_subject)
{
	manager->enemyHit(_subject);
};


GameManager::EnemyListener::~EnemyListener()
{};



void GameManager::loadVariables(ScriptNodePtr _data)
{
	ScriptNodePtr variables = ScriptLoader::getSingleton().getChildNodeByName(_data, VarVariables);
	assert(variables || "no variables node found");

	enemySpeedBase = std::stof(variables->getValue(VarEnemySpeedBase));
	enemySpeedMultiplier = std::stof(variables->getValue(VarEnemySpeedMultiplier));
	enemyCountBase = std::stof(variables->getValue(VarEnemyCountBase));
	enemyCountMultiplier = std::stof(variables->getValue(VarEnemyCountMultiplier));
	enemyStartingSpawnCount = std::stof(variables->getValue(VarEnemySpawnCount));
	enemyStartingSpawnMultiplier = std::stof(variables->getValue(VarEnemySpawnMultiplier));
	enemySpawnTimer = std::stof(variables->getValue(VarEnemySpawnTimer));
	enemySpawnTimerMultiplier = std::stof(variables->getValue(VarEnemySpawnTimerMultiplier));


	ScriptNodePtr menus = ScriptLoader::getSingleton().getChildNodeByNameAndValue(_data, "scene", "name", VarMenuScene);
	assert(menus || "no menu node found");
	setupInputControls(menus);


	ScriptNodePtr stages = ScriptLoader::getSingleton().getChildNodeByName(_data, "stages");
	ScriptNodeListPtr stageList = stages->getChildList();
	for (auto it = stageList->begin(), itEnd = stageList->end(); it != itEnd; ++it)
	{
		if (!(*it)->getName().compare("scene"))
			stageDataArr.push_back(*it);
	}

	stageDataArr.shrink_to_fit();
};


void GameManager::setupInputControls(ScriptNodePtr _data)
{
	// fill up actions
	controlsListenersMap.insert({inputStartGameButtonStr, std::make_unique<GameControl>(this, &GameManager::startGame)});
	controlsListenersMap.insert({inputSettingsButtonStr, std::make_unique<ActionMenuChange>(this, false, true, settingsMenuNodeStr)});
	controlsListenersMap.insert({inputExitButtonStr, std::make_unique<GameControl>(this, &GameManager::shutdown)});
	controlsListenersMap.insert({inputBackToMenuButtonStr, std::make_unique<ActionMenuChange>(this, true, false, "")});
	controlsListenersMap.insert({inputMenuButtonStr, std::make_unique<ActionMenuChange>(this, false, true, ingameMenuNodeStr)});
	controlsListenersMap.insert({inputResetButtonStr, std::make_unique<GameControl>(this, &GameManager::resetGame)});
	controlsListenersMap.insert({inputResumeButtonStr, std::make_unique<ActionMenuChange>(this, true, false, "")});
	controlsListenersMap.insert({inputToMainMenuButtonStr, std::make_unique<GameControl>(this, &GameManager::closeGame)});
	// setup inputs after loading scenes

	//menuScene
	//inputManager

	// projection space

	ScriptLoader &loader = ScriptLoader::getSingleton();

	ScriptNodeListPtr controlsList = _data->getChildList();
	for (auto it = controlsList->begin(), itEnd = controlsList->end(); it != itEnd; ++it)
	{
		ScriptNodePtr node = *it;

		TOUCH_CONTROL_TYPE controlType = loader.parseTouchControlType(node);
		std::string controlName = loader.parseTouchControlName(node);
		std::string controlSet = loader.parseTouchControlSet(node);
		auto shape = ShapeFactory::parseShape(loader.getTouchControlShapeNode(node));

		std::unique_ptr<TouchControl> control;

		switch (controlType)
		{
			case TCT_BUTTON:
			{
				control = std::make_unique<TouchButton>(controlName, std::move(shape));
				break;
			}

			case TCT_AREA:
			{
				control = std::make_unique<TouchArea>(controlName, std::move(shape));
				break;
			}

			case TCT_OBJECT_BUTTON:
			{
				MovableObject *attachedObject = menuScene->getObjectByName(loader.parseTouchControlAttachedToObject(node));
				Camera *controlsCamera = menuScene->getCurrentCamera();

				control = std::make_unique<ObjectAttachedTouchButton>(controlName, attachedObject, std::move(shape), controlsCamera);
				break;
			}
		}

		auto listnerIt = controlsListenersMap.find(controlName);
		if (listnerIt != controlsListenersMap.end())
			control->registerListener((*listnerIt).second.get());

		inputManager->registerControl(controlSet, std::move(control));
	}
};


void GameManager::popMenu()
{
	if (!menuStack.empty())
	{
		SceneNode *menuNode = menuScene->getNodeByName(menuStack.top());
		if (menuNode)
			menuNode->setEnabled(false);
		menuStack.pop();
	}

	if (!menuStack.empty())
	{
		SceneNode *menuNode = menuScene->getNodeByName(menuStack.top());
		if (menuNode)
			menuNode->setEnabled(true);

		inputManager->activateControlSet(menuStack.top());
	}
	else
		inputManager->deactivate();
};


void GameManager::pushMenu(std::string _newMenuName)
{
	if (!menuStack.empty())
	{
		SceneNode *menuNode = menuScene->getNodeByName(menuStack.top());
		if (menuNode)
			menuNode->setEnabled(false);
	}

	inputManager->activateControlSet(_newMenuName);
	SceneNode *menuNode = menuScene->getNodeByName(_newMenuName);
	if (menuNode)
		menuNode->setEnabled(false);

	menuStack.push(_newMenuName);
};



void GameManager::loadNextStage()
{
	++currentStage.stageLevel;

	int newStageIndex;
	while (stageDataArr.size() > 1 && (newStageIndex = std::rand() % stageDataArr.size()) == currentStage.lastStageIndex);
	auto stageNode = stageDataArr[currentStage.lastStageIndex = newStageIndex];

	currentStage.enemySpeed = enemySpeedBase * std::pow(enemySpeedMultiplier, (float)currentStage.stageLevel);
	currentStage.enemyCount = std::ceil(enemyCountBase * std::pow(enemyCountMultiplier, (float)currentStage.stageLevel));
	currentStage.startingSpawnCount = std::ceil(enemyStartingSpawnCount * std::pow(enemyStartingSpawnMultiplier, (float)currentStage.stageLevel));
	currentStage.spawnTimer = enemySpawnTimer * std::pow(enemySpawnTimerMultiplier, (float)currentStage.stageLevel);

	// clear last
	gameScene->clearScene();
	currentStage.aliveEnemyList.clear();
	currentStage.deadEnemyList.clear();
	currentStage.enemiesSpawned = 0;


	currentStage.aliveEnemyList.clear();
	currentStage.deadEnemyList.clear();

	if (!currentStage.player)
		currentStage.player = std::make_unique<Player>();

	if (!currentStage.stage)
		currentStage.stage = std::make_unique<Stage>();

	ScriptNodePtr playerSceneNode = ScriptLoader::getSingleton().getChildNodeByNameAndValue(stageNode, "node", "name", sceneNodePlayer);
	currentStage.player->create(inputManager, gameScene, playerSceneNode);

	ScriptNodePtr stageSceneNode = ScriptLoader::getSingleton().getChildNodeByNameAndValue(stageNode, "node", "name", sceneNodeStage);
	currentStage.stage->create(gameScene, stageSceneNode);


	ScriptNodePtr enemyNode = ScriptLoader::getSingleton().getChildNodeByNameAndValue(stageNode, "node", "name", sceneNodeEnemy);

	currentStage.deadEnemyList.push_back(std::move(std::make_unique<Enemy>()));
	currentStage.deadEnemyList.front()->create(gameScene, enemyNode, currentStage.stage.get());
	for (int i = 1; i < currentStage.enemyCount; ++i)
		currentStage.deadEnemyList.push_back(std::move(std::make_unique<Enemy>(*currentStage.deadEnemyList.front())));
};


void GameManager::enemyHit(Enemy *_subject)
{
	if (_subject->getLevel() != Enemy::LEVEL::LAST_LEVEL)
	{
		std::unique_ptr<Enemy> enemyChild1 = std::make_unique<Enemy>(*_subject);
		std::unique_ptr<Enemy> enemyChild2 = std::make_unique<Enemy>(*_subject);

		float offsetX = 0.0f; // get size of new enemies

		enemyChild1->lowerLevel();
		auto listener1 = std::make_unique<EnemyListener>(this);
		enemyChild1->registerListener(std::move(listener1));

		enemyChild2->lowerLevel();
		auto listener2 = std::make_unique<EnemyListener>(this);
		enemyChild2->registerListener(std::move(listener1));

		float radius = enemyChild1->getRadius();
		radius += 0.001f;

		Vector2 position = _subject->getPosition();
		Vector2 direction = _subject->getDirection();
		direction.x = -std::abs(direction.x); // direction to left

		enemyChild1->spawn({position.x - radius, position.y}, direction);
		currentStage.aliveEnemyList.insert(std::move(enemyChild1));

		enemyChild2->spawn({position.x + radius, position.y}, {-direction.x, direction.y});
		currentStage.aliveEnemyList.insert(std::move(enemyChild2));
	}

	auto subjectIt = std::find_if(currentStage.aliveEnemyList.begin(), currentStage.aliveEnemyList.end(), [_subject] (auto up) { return up == _subject;});
	currentStage.deadEnemyList.insert(currentStage.deadEnemyList.end(), std::move(currentStage.aliveEnemyList.extract(subjectIt).value()));

	_subject->kill();

	addPoints();
};
