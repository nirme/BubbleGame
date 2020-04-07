#include "Engine.h"

namespace core
{


	Engine::Engine(android_app* _androidApp) :
		androidApp(_androidApp),
		initialized(false),
		appDataProvider(nullptr),
		controllerManager(nullptr),
		loader(nullptr),
		resourceManager(nullptr),
		renderer(nullptr),
		soundSystem(nullptr),
		physicsSystem(nullptr),
		inputManager(nullptr),
		//mainScene(nullptr),
		frameTime()
	{};


	_2d::SceneManager* Engine::createScene(const std::string &_name, float _sceneScale, int _pos)
	{
		assert(namedScenes.find(_name) == namedScenes.end() && "cannot create multiple scenes with the same name");

		std::unique_ptr<_2d::SceneManager> newScene = std::make_unique<_2d::SceneManager>(_name);
		newScene->setupManager(renderer.get(), renderer->getScreenWidth(), renderer->getScreenHeight(), _sceneScale);

		_2d::SceneManager *output = newScene.get();

		namedScenes.emplace(_name, newScene.get());

		if (_pos < 0 || _pos >= scenes.size())
			scenes.push_back(std::move(newScene));

		else
			scenes.insert(scenes.begin() + _pos, std::move(newScene));


		return output;
	};

	void Engine::destroyScene(const std::string &_name)
	{
		NamedScenesMap::iterator itScene = namedScenes.find(_name);
		assert(itScene != namedScenes.end() && "cannot destroy scene, scene doesn't exist");

		std::remove_if(scenes.begin(), scenes.end(), [val = (*itScene).second](ScenesList::value_type &_iter) { return val == _iter.get();});
		namedScenes.erase(itScene);
	};


	_2d::SceneManager* Engine::getScene(const std::string &_name)
	{
		assert(namedScenes.find(_name) != namedScenes.end() && "cannot find scene, scene doesn't exist");
		return (*namedScenes.find(_name)).second;
	};


	void Engine::setInputManager(InputManager *_manager)
	{
		inputManager = _manager;
	};


	void Engine::onActivate()
	{
		Logger::getSingleton().write("onActivate event called");
		frameTime.reset();
	};


	void Engine::onDeactivate()
	{
		Logger::getSingleton().write("onDeactivate event called");
	};


	void Engine::onStep()
	{
		if (!initialized)
			return;

		// update all controllers
		frameTime.update();
		controllerManager->addFrameTime(frameTime.getLastUpdateTime<double>());
		controllerManager->updateControllers();

		physicsSystem->progressTime(frameTime.getLastUpdateTime<float>());


		renderer->beforeRendering();

		// render scene
		for (unsigned int i = 0, iEnd = scenes.size(); i < iEnd; ++i)
		{
			if (scenes[i])
				scenes[i]->renderScene();
		}

		renderer->afterRendering();
	};



	void Engine::onInitWindow()
	{
		Logger::getSingleton().write("onInitWindow event called");

		loader = std::unique_ptr<ScriptLoader>(new ScriptLoader());
		loader->registerParser(std::make_shared<XmlScriptParser>());
		appDataProvider = std::make_shared<AndroidDataProvider>(androidApp->activity->assetManager);


		// systems
		controllerManager = std::unique_ptr<ControllerManager>(new ControllerManager());
		controllerManager->initialize();


		renderer = std::unique_ptr<RenderSystem>(new RenderSystem());
		renderer->enableMultisampling(true);
		renderer->initialize(androidApp);
		// default renderer setup
		{
			renderer->getStateCashe()->setFaceCulling(false);
			renderer->getStateCashe()->setBlending(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD);
			renderer->getStateCashe()->setDepthTest(false);
			renderer->getStateCashe()->setScissorTest(false);
			renderer->getStateCashe()->applyState();
		}

		inputManager->setScreenSize(renderer->getScreenWidth(), renderer->getScreenHeight());


		soundSystem = std::unique_ptr<SoundSystem>(new SoundSystem());
		soundSystem->initialize(44100, 16, 2, 12); // 12 channels


		physicsSystem = std::unique_ptr<_2d::PhysicsSystem>(new _2d::PhysicsSystem());


		resourceManager = std::unique_ptr<ResourceSystem>(new ResourceSystem());
		resourceManager->registerDataProvider(appDataProvider);

		// register res managers and parse configuration
		resourceManager->registerResourceManager("texture", new TextureManager(renderer.get()));
		resourceManager->registerResourceManager("shader", new ShaderManager(renderer.get()));
		resourceManager->registerResourceManager("shading_program", new ShadingProgramManager(renderer.get()));
		resourceManager->registerResourceManager("sprite", new ImageSpriteManager);
		resourceManager->registerResourceManager("font", new SpritedFontManager);
		resourceManager->registerResourceManager("sound", new SoundManager(soundSystem.get()));


		frameTime.reset();
		initialized = true;
	};


	void Engine::onTermWindow()
	{
		Logger::getSingleton().write("onTermWindow event called");

		if (renderer)
			renderer->uninitialize();

		initialized = false;
	};

	void Engine::onGainedFocus()
	{
		Logger::getSingleton().write("onGainedFocus event called");
	};

	void Engine::onLostFocus()
	{
		Logger::getSingleton().write("onLostFocus event called");
	};


	void Engine::onConfigChanged()
	{};


	void Engine::onLowMemory()
	{};


	void Engine::onStart()
	{
		Logger::getSingleton().write("onStart event called");
	};


	void Engine::onResume(void* pData, size_t pSize)
	{
		Logger::getSingleton().write("onResume event called");
		if (initialized)
		{
			soundSystem->resume();
		}
	};

	void Engine::onSaveState(void** pData, size_t* pSize)
	{
		Logger::getSingleton().write("onSaveState event called");
	};

	void Engine::onPause()
	{
		Logger::getSingleton().write("onPause event called");

		soundSystem->pause();
	};

	void Engine::onStop()
	{
		Logger::getSingleton().write("onStop event called");

		// destroy physics system
		physicsSystem.reset();

		// destroy all scenes
		scenes.clear();
		namedScenes.clear();

		// stop any sound that might be still playing
		soundSystem->stop();

		// release all resources
		resourceManager->releaseAllResourceSystems();
		resourceManager.reset();

		// destroy sound system
		soundSystem->uninitialize();
		soundSystem.reset();

		// destroy renderer
		renderer->uninitialize();
		renderer.reset();

		// destroy controllers and script loader
		controllerManager.reset();
		loader.reset();
	};


	void Engine::onDestroy()
	{
		Logger::getSingleton().write("onDestroy event called");
	};

}
