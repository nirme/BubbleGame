#pragma once

#include <unordered_set>
#include <list>
#include <memory>
#include <stack>

#include "GameStrings.h"

#include "Stage.h"
#include "Player.h"
#include "Enemy.h"

#include "core/Engine.h"
#include "MusicController.h"

using namespace core;
using namespace core::_2d;


class GameManager
{
protected:

	constexpr static char *VarVariables = "variables";
	constexpr static char *VarStages = "stages";
	constexpr static char *VarMenuScene = "menu_scene";

	constexpr static char *VarEnemySpeedBase = "enemy_speed_base";
	constexpr static char *VarEnemySpeedMultiplier = "enemy_speed_multiplier";
	constexpr static char *VarEnemyCountBase = "enemy_count_base";
	constexpr static char *VarEnemyCountMultiplier = "enemy_count_multiplier";
	constexpr static char *VarEnemySpawnCount = "enemy_starting_spawn_count";
	constexpr static char *VarEnemySpawnMultiplier = "enemy_starting_spawn_multiplier";
	constexpr static char *VarEnemySpawnTimer = "enemy_spawn_timer";
	constexpr static char *VarEnemySpawnTimerMultiplier = "enemy_spawn_timer_multiplier";

	constexpr static int playerLivesBase = 3;

public:

	class GameControl : public TouchControl::Listener
	{
	protected:
		typedef void (GameManager::*Function)();
		GameManager *parent;
		Function memberFunc;

	public:
		GameControl(GameManager *_parent, Function _memberFunc);
		void onButtonClicked();
	};

	class ActionMenuChange : public TouchControl::Listener
	{
	protected:
		GameManager *parent;
		bool popFromStack;
		bool pushToStack;
		std::string newMenuName;

	public:
		ActionMenuChange(GameManager *_parent, bool _popFromStack, bool _pushToStack, std::string _newMenuName);
		void onButtonClicked();
	};


	class EnemyListener : public Enemy::Listener
	{
	protected:
		GameManager *manager;
	public:
		EnemyListener(GameManager *_manager);
		void onLaserHit(Enemy *_subject);
		~EnemyListener();
	};



	protected:

	enum State
	{
		S_STARTING,
		S_START_MENU,
		S_INGAME_MENU,
		S_INGAME,
		S_ENDING,
	} currentState;

	bool paused;

	SceneManager *gameScene;
	SceneManager *menuScene;

	std::stack<std::string> menuStack;

	std::unordered_map<std::string, std::unique_ptr<TouchControl::Listener>> controlsListenersMap;

	std::unique_ptr<MusicController> musicController;
	InputManager *inputManager;



	struct StageState
	{
		std::unique_ptr<Player> player;
		std::unique_ptr<Stage> stage;

		// current enemies
		std::unordered_set<std::unique_ptr<Enemy>> aliveEnemyList;
		std::list<std::unique_ptr<Enemy>> deadEnemyList;


		int lastStageIndex;
		int stageLevel;

		float enemySpeed;
		int enemyCount;
		int startingSpawnCount;
		float spawnTimer;


		//state values
		int enemiesSpawned;
		int playerLives;
		unsigned int points;
	} currentStage;



	// game variables
	float enemySpeedBase;
	float enemySpeedMultiplier;
	float enemyCountBase;
	float enemyCountMultiplier;
	float enemyStartingSpawnCount;
	float enemyStartingSpawnMultiplier;
	float enemySpawnTimer;
	float enemySpawnTimerMultiplier;

	//stages
	std::vector<ScriptNodePtr> stageDataArr;


////////////////////////////////////////////////
	void loadVariables(ScriptNodePtr _data);
	void setupInputControls(ScriptNodePtr _data);

	void popMenu();
	void pushMenu(std::string _newMenuName);

	void loadNextStage();


	void startGame()
	{
		// show loading screen


		currentStage.stageLevel = -1;
		currentStage.lastStageIndex = -1;
		currentStage.playerLives = playerLivesBase;
		currentStage.points = 0;

		loadNextStage();



	};

	void shutdown()
	{};

	void resetGame()
	{};

	void closeGame()
	{};


	void addPoints()
	{
		++currentStage.points;
	};

	void pauseGame()
	{
		if (currentState != S_INGAME || paused)
			return;

		currentStage.player->pause();
		currentStage.stage->pause();

		for (auto it = currentStage.aliveEnemyList.begin(), itEnd = currentStage.aliveEnemyList.end(); it != itEnd; ++it)
			(*it)->pause();

		PhysicsSystem::getSingleton().setEnabled(false);

		paused = true;
	};


public:

	void enemyHit(Enemy *_subject);



};