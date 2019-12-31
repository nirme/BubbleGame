#include "ControllerManager.h"


template<>
core::ControllerManager* Singleton<core::ControllerManager>::impl = nullptr;

namespace core
{
	ControllerManager::ControllerManager() :
			frameTimeValue(nullptr)
	{};

	void ControllerManager::initialize()
	{
		frameTimeValue = std::make_shared<FrameTimeControllerValue>(FrameTimeControllerValue(1.0f));
	};


	SharedFrameTimeControllerValuePtr ControllerManager::getFrameTimeControllerValues()
	{
		return frameTimeValue;
	};


	ControllerPtr ControllerManager::createController(SharedControllerValuePtr _source, SharedControllerValuePtr _destination, SharedControllerFuncPtr _function)
	{
		ControllerPtr controller = std::make_shared<Controller>(_source, _destination, _function);
		activeControllerList.insert(controller);
		return controller;
	};

	ControllerPtr ControllerManager::createFrameTimeController(SharedControllerValuePtr _destination, SharedControllerFuncPtr _function)
	{
		return createController(getFrameTimeControllerValues(), _destination, _function);
	};


	void ControllerManager::removeController(ControllerPtr _controller)
	{
		activeControllerList.erase(_controller);
	};


	void ControllerManager::addFrameTime(float _time)
	{
		frameTimeValue->set(_time);
	};

	float ControllerManager::getFrameTime()
	{
		return frameTimeValue->get();
	};


	float ControllerManager::getElapsedTime()
	{
		return frameTimeValue->getElapsedTime();
	};


	void ControllerManager::setTimeSpeed(float _timeSpeed)
	{
		frameTimeValue->setTimeSpeed(_timeSpeed);
	};

	float ControllerManager::getTimeSpeed()
	{
		return frameTimeValue->getTimeSpeed();
	};


	void ControllerManager::updateControllers()
	{
		for (auto it = activeControllerList.begin(), itEnd = activeControllerList.end(); it != itEnd; ++it)
		{
			(*it)->update();
		}
	};

}
