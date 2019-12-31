#pragma once

#include "Engine.h"
#include "InputHandler.h"

namespace core
{
	class Application
	{
	protected:


		bool isActive;
		android_app* androidApp;

		Engine* engine;
		InputHandler* inputHandler;


	public:

		Application(android_app* _androidApp, Engine* _engine, InputHandler* _inputHandler) :
			isActive(false), androidApp(_androidApp), engine(_engine), inputHandler(_inputHandler)
		{
			androidApp->onAppCmd = onApplicationCommand;
			androidApp->onInputEvent = onInputEvent;
            androidApp->userData = this;
		};


		int getScreenDensity(AConfiguration *_config)
		{
			assert(_config && "AConfiguration pointer cannot be null");

			int32_t densityType = AConfiguration_getDensity(_config);

			switch (densityType)
			{
			case ACONFIGURATION_DENSITY_DEFAULT:
			case ACONFIGURATION_DENSITY_ANY:
			case ACONFIGURATION_DENSITY_NONE:
				return (int)ACONFIGURATION_DENSITY_MEDIUM;

			default:
				return (int)densityType;
			}
		};


		void run()
		{
			int ret;
			int events;
			android_poll_source* source;

			while (true)
			{
				// Read all pending events.
				// if app not active then wait indefinitely for next event
				while ((ret = ALooper_pollAll(isActive ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
				{
					// process events if any
					if (source)
					{
						source->process(androidApp, source);
					}

					// end loop if exiting app
					if (androidApp->destroyRequested)
					{
						return;
					}
				}

				engine->onStep();
			}


		};


		void activate()
		{
			engine->onActivate();
			isActive = true;
		};

		void deactivate()
		{
			engine->onDeactivate();
			isActive = false;
		};



		static void onApplicationCommand(android_app* app, int32_t cmd)
		{
			Application& application = *(Application*)app->userData;
			application.processAppEvent(cmd);
		};



		static int32_t onInputEvent(android_app* app, AInputEvent* event)
		{
			Application& application = *(Application*)app->userData;
			return application.processInputEvent(event);
		};




		void processAppEvent(int32_t cmd)
		{
			switch (cmd)
			{
			case APP_CMD_INIT_WINDOW:
				engine->onInitWindow();
				return;

			case APP_CMD_TERM_WINDOW:
				engine->onTermWindow();
				deactivate();
				return;

			case APP_CMD_GAINED_FOCUS:
				activate();
				engine->onGainedFocus();
				return;

			case APP_CMD_LOST_FOCUS:
				engine->onLostFocus();
				deactivate();
				return;

			case APP_CMD_CONFIG_CHANGED:
				engine->onConfigChanged();
				return;

			case APP_CMD_LOW_MEMORY:
				engine->onLowMemory();
				return;

			case APP_CMD_START:
				engine->onStart();
				return;

			case APP_CMD_RESUME:
				engine->onResume(androidApp->savedState, androidApp->savedStateSize);
				return;

			case APP_CMD_SAVE_STATE:
				engine->onSaveState(&(androidApp->savedState), &(androidApp->savedStateSize));
				return;

			case APP_CMD_PAUSE:
				engine->onPause();
				deactivate();
				return;

			case APP_CMD_STOP:
				engine->onStop();
				return;

			case APP_CMD_DESTROY:
				engine->onDestroy();
				deactivate();
				return;
			}
		};


		int32_t processInputEvent(AInputEvent* event)
		{
			if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
			{
				switch (AInputEvent_getSource(event))
				{
				case AINPUT_SOURCE_TOUCHSCREEN:
					inputHandler->onTouchEvent(event);
					break;

					//case AINPUT_SOURCE_DPAD:
					//case AINPUT_SOURCE_GAMEPAD:
					//case AINPUT_SOURCE_JOYSTICK:
					//case AINPUT_SOURCE_MOUSE:
					//case AINPUT_SOURCE_KEYBOARD:
				}
			}
			//case AINPUT_EVENT_TYPE_KEY:
			else
			{}

			return 0;
		};


	};

}
