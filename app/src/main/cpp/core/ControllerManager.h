#pragma once

#include <memory>
#include <set>

#include "SingletonTemplate.h"
#include "Controller.h"
#include "ControllerValues.h"


namespace core
{


	class ControllerManager : public Singleton<ControllerManager>
	{
	protected:
		typedef std::set<ControllerPtr> ControllerList;
		//typedef std::unordered_set<ControllerPtr> ControllerList;
		//typedef std::vector<ControllerPtr> ControllerList;

		ControllerList activeControllerList;
		SharedFrameTimeControllerValuePtr frameTimeValue;


	public:

		ControllerManager();
		void initialize();

		SharedFrameTimeControllerValuePtr getFrameTimeControllerValues();

		ControllerPtr createController(SharedControllerValuePtr _source, SharedControllerValuePtr _destination, SharedControllerFuncPtr _function = nullptr);
		ControllerPtr createFrameTimeController(SharedControllerValuePtr _destination, SharedControllerFuncPtr _function = nullptr);

		void removeController(ControllerPtr _controller);

		void addFrameTime(float _time);
		float getFrameTime();

		float getElapsedTime();

		void setTimeSpeed(float _timeSpeed);
		float getTimeSpeed();

		void updateControllers();

	};
}
