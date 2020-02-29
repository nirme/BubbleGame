#pragma once

#include <thread>
#include <memory>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "ActivityHandler.h"

//#include "Exceptions.h"
//#include "Logger.h"

#include "RenderSystem.h"

#include "_2d/SceneManager.h"

#include "ControllerManager.h"

#include "DataProvider.h"
#include "AndroidDataProvider.h"
#include "XMLScriptParser.h"
#include "ScriptLoader.h"

#include "InputManager.h"
#include "_2d/ShapeRectangle.h"
#include "TouchControls.h"

#include "ResourceSystem.h"

#include "ShadingProgramManager.h"
#include "ImageSpriteManager.h"
#include "SpritedFontManager.h"
#include "TextureManager.h"
#include "SoundManager.h"

#include "SoundSystem.h"

#include "_2d/ViewPort.h"
#include "Timer.h"


#include "_2d/ParticleAffectors.h"
#include "_2d/ParticleEmitters.h"


namespace core
{

	class Engine : public ActivityHandler
	{
	private:
		android_app* androidApp;

		bool initialized;

		DataProviderPtr appDataProvider;

        std::unique_ptr<ControllerManager> controllerManager;
        std::unique_ptr<ScriptLoader> loader;
        std::unique_ptr<ResourceSystem> resourceManager;
		std::unique_ptr<RenderSystem> renderer;
		std::unique_ptr<SoundSystem> soundSystem;

		InputManager *inputManager;


		Timer frameTime;
        std::unique_ptr<_2d::SceneManager> mainScene;

	public:

		Engine(android_app* _androidApp) :
			androidApp(_androidApp),
			initialized(false),
            appDataProvider(nullptr),
            controllerManager(nullptr),
            loader(nullptr),
            resourceManager(nullptr),
            renderer(nullptr),
			soundSystem(nullptr),
            inputManager(nullptr),
            mainScene(nullptr),
            frameTime()
		{};


		void setInputManager(InputManager *_manager)
		{
			inputManager = _manager;
		};



		void onActivate() {};
		void onDeactivate() {};

        void onStep()
        {
            if (!initialized)
                return;

            // update all controllers
            frameTime.update();
            controllerManager->addFrameTime(frameTime.getLastUpdateTime<double>());
            controllerManager->updateControllers();



            renderer->beforeRendering();

            // render scene
            mainScene->renderScene();

            renderer->afterRendering();

        };



		void onInitWindow()
		{
            //controllerManager.release();
            controllerManager = std::unique_ptr<ControllerManager>(new ControllerManager());

            //loader.release();
            loader = std::unique_ptr<ScriptLoader>(new ScriptLoader());

            //resourceManager.release();
            resourceManager = std::unique_ptr<ResourceSystem>(new ResourceSystem());

            //renderer.release();
            renderer = std::unique_ptr<RenderSystem>(new RenderSystem());

			//soundSystem.release();
			soundSystem = std::unique_ptr<SoundSystem>(new SoundSystem());

            //mainScene.release();
            mainScene = std::unique_ptr<_2d::SceneManager>(new _2d::SceneManager());



			//
			// setup parser and data provider with specific impl

			loader->registerParser(std::make_shared<XmlScriptParser>());
			appDataProvider = std::make_shared<AndroidDataProvider>(androidApp->activity->assetManager);

			resourceManager->registerDataProvider(appDataProvider);


			// initialize systems required for resource loading
			renderer->setBackgroundColor(0.5f, 0.5f, 0.5f);
            renderer->enableMultisampling(true);
			renderer->initialize(androidApp);

			//soundSystem->setSoundConfig(44100, 16, 2);
            soundSystem->initialize();

{
	renderer->getStateCashe()->setFaceCulling(false);
	renderer->getStateCashe()->setBlending(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD);
	renderer->getStateCashe()->setDepthTest(false);
	renderer->getStateCashe()->setScissorTest(false);
	renderer->getStateCashe()->applyState();
}

			// register res managers and parse configuration

			resourceManager->registerResourceManager("texture", new TextureManager(renderer.get()));
			resourceManager->registerResourceManager("shader", new ShaderManager(renderer.get()));
			resourceManager->registerResourceManager("shading_program", new ShadingProgramManager(renderer.get()));
			resourceManager->registerResourceManager("sprite", new ImageSpriteManager);
			resourceManager->registerResourceManager("font", new SpritedFontManager);
			resourceManager->registerResourceManager("sound", new SoundManager(soundSystem.get()));

			resourceManager->parseConfiguration("GameResourceCfg.xml");

			// init controllers
            controllerManager->initialize();

            mainScene->setupManager(renderer.get(), renderer->getScreenWidth(), renderer->getScreenHeight(), (float)renderer->BaseScreenDensity / renderer->getScreenDensity());


			inputManager->setScreenSize(renderer->getScreenWidth(), renderer->getScreenHeight());





{
	_2d::SceneNode *node = mainScene->createNode("TestNode", nullptr);
	mainScene->getRootNode()->appendChild(node);

	node->setPosition({ 0.0f, 0.0f });
	node->setRotation(0.0f);
	node->setScale({ 1.0f });

    _2d::SingleSprite *spriteTest = nullptr;
    spriteTest = mainScene->createSingleSprite("TestSprite", nullptr);
    spriteTest->setEnabled(true);
    ShadingProgramManager::getSingleton().getByName("baseShader")->load();
    ImageSpriteManager::getSingleton().getByName("anim_1.bmp")->load();
    spriteTest->setMaterial(ShadingProgramManager::getSingleton().getByName("baseShader"), ImageSpriteManager::getSingleton().getByName("anim_1.bmp"));

    spriteTest->setPosition({0.0f});

    spriteTest->setPriority(127);
    spriteTest->setRotation(0.0f);
    spriteTest->setScale({1.0f});
    spriteTest->setSpriteCoords(SpriteCoords::SPRITE_SQUARE);

    node->appendObject(spriteTest);
/*
	class MoveListener : public TouchArea::Listener
	{
	public:
		_2d::MovableObject *object;
		Vector2 pos;
		void onPointerOnArea()
		{
		    object->setPosition(object->getPosition() + pos);
		};
		void onPointerOffArea()
		{
		    object->setPosition(object->getPosition() + pos);
		};
	};

	// btn1
	MoveListener *mList1 = new MoveListener();
	mList1->object = spriteTest;
	mList1->pos.x = -0.3f;

	_2d::Shape *btn1Shape = new _2d::Rectangle(Vector2(-10.0f, 10.0f), Vector2(-0.6f, 10.0f), Vector2(-0.6f, -10.0f), Vector2(-10.0f, -10.0f));
	TouchArea *buttonLeft = new TouchArea(btn1Shape);
	buttonLeft->registerListener(mList1);

	inputManager->registerControl("base_control", buttonLeft);


	// btn2
	MoveListener *mList2 = new MoveListener();
	mList2->object = spriteTest;
	mList2->pos.x = 0.3f;

	_2d::Shape *btn2Shape = new _2d::Rectangle(Vector2(10.0f, 10.0f), Vector2(0.6f, 10.0f), Vector2(0.6f, -10.0f), Vector2(10.0f, -10.0f));
	TouchArea *buttonRight = new TouchArea(btn2Shape);
	buttonRight->registerListener(mList2);

	inputManager->registerControl("base_control", buttonRight);

	inputManager->activateControlSet("base_control");

*/




// Particle system test

_2d::SceneNode *node2 = mainScene->createNode("ParticleSystemTest", nullptr);
mainScene->getRootNode()->appendChild(node2);

node2->setPosition({ -5.0f, 0.0f });
node2->setRotation(0.0f);
node2->setScale({ 1.0f });


_2d::ParticleSystem* particleTest = mainScene->createParticleSystem("TestParticleSystem", nullptr);
particleTest->setEnabled(true);
node2->appendObject(particleTest);
particleTest->setEnabled(true);

particleTest->setMaterial(
        ShadingProgramManager::getSingleton().getByName("particleShader"),
        TextureManager::getSingleton().getByName("explosion.bmp")
);


{
    _2d::PointRandomParticleEmitter *emiter = new _2d::PointRandomParticleEmitter(particleTest);
	//_2d::RandomParticleEmitter *emiter = new _2d::RandomParticleEmitter(particleTest);
	//emiter->setEmissionPosition({-1.0f, -1.0f}, {1.0f, 1.0f});
	emiter->setEmissionPosition({-0.1f, -0.1f}, {0.1f, 0.1f});
	emiter->setEmissionRotation(0.0f, 2.0f * M_PI);
	emiter->setEmissionSize(0.75f, 1.5f);

	//emiter->setEmissionDirection(0.0f, M_PI * 2, 1.25f, 1.75f);
	emiter->setEmissionDirection(M_PI * 0.5f - 0.1f, M_PI * 0.5f + 0.1f);
    emiter->setEmissionLinearSpeed(1.5f, 2.5f);
	emiter->setEmissionRotationalSpeed(0.0f, 0.3f);
	emiter->setEmissionSizeChangeSpeed(0.0f, 0.3f);

	emiter->setEmissionTimeAlive(2.0f, 3.5f);
	emiter->setEmissionRate(60.0f);

	particleTest->addEmiter(emiter);



class EmissionListener : public TouchArea::Listener
{
public:
    _2d::ParticleEmitter *object;
    int emisionChange;
    float speedChange;
    void onPointerOnArea()
    {
        object->setEmissionRate(object->getEmissionRate() + emisionChange);
        object->setEmissionLinearSpeed(object->getEmissionMinLinearSpeed() + speedChange,
        		object->getEmissionMaxLinearSpeed() + speedChange
		);
        Logger::getSingleton().write(std::to_string(object->getEmissionRate()) );
    };
    void onPointerOffArea()
    {};
};

// btn1
EmissionListener *mList1 = new EmissionListener();
mList1->object = emiter;
mList1->emisionChange = -10;
mList1->speedChange = -0.1f;

_2d::Shape *btn1Shape = new _2d::Rectangle(Vector2(-10.0f, 10.0f), Vector2(-0.6f, 10.0f), Vector2(-0.6f, -10.0f), Vector2(-10.0f, -10.0f));
TouchArea *buttonLeft = new TouchArea(btn1Shape);
buttonLeft->registerListener(mList1);
inputManager->registerControl("base_control", buttonLeft);

// btn2
EmissionListener *mList2 = new EmissionListener();
mList2->object = emiter;
mList2->emisionChange = 10;
mList2->speedChange = 0.1f;

_2d::Shape *btn2Shape = new _2d::Rectangle(Vector2(10.0f, 10.0f), Vector2(0.6f, 10.0f), Vector2(0.6f, -10.0f), Vector2(10.0f, -10.0f));
TouchArea *buttonRight = new TouchArea(btn2Shape);
buttonRight->registerListener(mList2);

inputManager->registerControl("base_control", buttonRight);

inputManager->activateControlSet("base_control");





}

_2d::ParticleSpriteAgeAffector *imgAffector = new _2d::ParticleSpriteAgeAffector(particleTest);
particleTest->addAffector(imgAffector);

_2d::ParticleSpeedAffector *speedAffector = new _2d::ParticleSpeedAffector(particleTest);
speedAffector->setDirection({0.0f, -0.6f});
speedAffector->setOperation(_2d::ParticleSpeedAffector::SMO_ADD);
particleTest->addAffector(speedAffector);

{
	std::string texName = "explosion.bmp#";
	for (unsigned int i = 0; i <= 40; ++i)
	{
		std::string spriteName = texName + std::to_string(i);
		particleTest->addSprite(ImageSpriteManager::getSingleton().getByName(spriteName));
		float time = 1.0f - (i / 41.0f);
		time = std::pow(time, 1.3f);
		imgAffector->setSprite(time,spriteName);
	}
}

particleTest->setPriority(0xFF);
particleTest->setSpeedFactor(1.0f);
particleTest->initSystem(10000);

particleTest->setPosition({0.0f, 0.0f});
particleTest->setRotation(0.0f);
particleTest->setScale(1.0f);


}



			frameTime.reset();
            initialized = true;

SoundPtr testSound = SoundManager::getSingleton().getByName("test.mp3");
testSound->load();
SoundSystem::getSingleton().playSound(0, testSound);

//SoundManager::getSingleton().getByName("test.ogg")->load();
//SoundManager::getSingleton().getByName("test.flac")->load();
//SoundManager::getSingleton().getByName("test.aac")->load();
//SoundManager::getSingleton().getByName("test.ac3")->load();

//soundSystem->runTest(testSound);

		};


		void onTermWindow()
		{

            renderer->uninitialize();

            initialized = false;
		};

		void onGainedFocus() {};

		void onLostFocus() {};
		void onConfigChanged() {};

		void onLowMemory() {};
		void onStart()
		{};


		void onResume(void* pData, size_t pSize)
		{

		};

		void onSaveState(void** pData, size_t* pSize) {};
		void onPause() {};

		void onStop()
		{
            resourceManager->releaseAllResourceSystems();
		};


		void onDestroy()
		{

		};

	};

}
