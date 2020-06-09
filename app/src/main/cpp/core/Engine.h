#pragma once

#include <thread>
#include <algorithm>
#include <memory>
#include <utility>
#include <unordered_map>
#include <vector>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <android_native_app_glue.h>

#include "ActivityHandler.h"

#include "RenderSystem.h"

#include "_2d/SceneManager.h"

#include "ControllerManager.h"

#include "DataProvider.h"
#include "AndroidDataProvider.h"
#include "XMLScriptParser.h"
#include "ScriptLoader.h"

#include "InputManager.h"
#include "_2d/shapes/Rectangle.h"
#include "TouchControls.h"

#include "ResourceSystem.h"

#include "ShadingProgramManager.h"
#include "ImageSpriteManager.h"
#include "SpritedFontManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "_2d/PhysicsSystem.h"

#include "SoundSystem.h"

#include "_2d/ViewPort.h"
#include "Timer.h"


#include "_2d/ParticleAffectors.h"
#include "_2d/ParticleEmitters.h"

#include "_2d/PhysicsAffectors.h"



namespace core
{

	class Engine : public ActivityHandler
	{
	protected:
		android_app* androidApp;

		bool initialized;

		DataProviderPtr appDataProvider;

        std::unique_ptr<ControllerManager> controllerManager;
        std::unique_ptr<ScriptLoader> loader;
        std::unique_ptr<ResourceSystem> resourceManager;
		std::unique_ptr<RenderSystem> renderer;
		std::unique_ptr<SoundSystem> soundSystem;
		std::unique_ptr<_2d::PhysicsSystem> physicsSystem;

		InputManager *inputManager;


		Timer frameTime;
        //std::unique_ptr<_2d::SceneManager> mainScene;
        typedef std::vector<std::unique_ptr<_2d::SceneManager>> ScenesList;
        typedef std::unordered_map<std::string, _2d::SceneManager*> NamedScenesMap;

		ScenesList scenes;
		NamedScenesMap namedScenes;

	public:

		Engine(android_app* _androidApp);

		_2d::SceneManager* createScene(const std::string &_name, float _sceneScale = 1.0f, int _pos = -1);
		void destroyScene(const std::string &_name);
		_2d::SceneManager* getScene(const std::string &_name);

		void setInputManager(InputManager *_manager);

		virtual void onActivate();
		virtual void onDeactivate();
		virtual void onStep();

		virtual void onInitWindow();
		virtual void onTermWindow();

		virtual void onGainedFocus();
		virtual void onLostFocus();

		virtual void onConfigChanged();
		virtual void onLowMemory();

		virtual void onStart();
		virtual void onResume(void* pData, size_t pSize);
		virtual void onSaveState(void** pData, size_t* pSize);
		virtual void onPause();
		virtual void onStop();

		virtual void onDestroy();

	};

}
