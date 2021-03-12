#include "InputManager.h"


namespace core
{

	PointerPosition InputManager::createPointerPosition(float _x, float _y)
	{


		return PointerPosition{
					_x,
					_y,
					(_x * inverseScreenSize.x * 2.0f) - 1.0f,
					(_y * inverseScreenSize.y * -2.0f) + 1.0f,
					(_x - (screenSize.x * 0.5f)) * inverseScreenSize.y *2.0f
		};
	};


	void InputManager::removeFromActivePointers(TouchControl *_control)
	{
		std::vector<int32_t> idToRemove;
		for (auto it = activePointers.begin(), itEnd = activePointers.end(); it != itEnd; ++it)
			if ((*it).second.affectedControl == _control)
				idToRemove.push_back((*it).first);

		while (idToRemove.size())
		{
			activePointers.erase(idToRemove.back());
			idToRemove.pop_back();
		}

	};


	InputManager::InputManager() :
		active(false),
		screenSize{ 1, 1 },
		inverseScreenSize{ 1.0f, 1.0f },
		currentSetName(emptySetName),
		currentSet(nullptr)
	{};


	void InputManager::setScreenSize(unsigned short _width, unsigned short _height)
	{
		screenSize = { _width, _height };
		inverseScreenSize = Vector2(1.0f / screenSize.x, 1.0f / screenSize.y);
	};


	void InputManager::registerControl(const std::string &_setName, std::unique_ptr<TouchControl> _control)
	{
		assert(_control && "cannot add nullptr as touch control");

		auto res = controlSets.find(_setName);
		if (res == controlSets.end())
			auto insertRes = controlSets.insert(std::make_pair(_setName, TouchControlList()));

		TouchControlList &list = (*res).second;

		auto it = std::find(list.begin(), list.end(), _control);
		if (it == list.end())
			list.push_back(std::move(_control));
	};


	TouchControl *InputManager::getControlByName(const std::string &_controlName)
	{
		for (TouchControlSetsIterator it = controlSets.begin(), itEnd = controlSets.end(); it != itEnd; ++it)
		{
			TouchControlList &list = (*it).second;
			auto controlIt = std::find_if(list.begin(), list.end(), [_controlName] (TouchControl* _control) { return !_controlName.compare(_control->getName()); });
			if (controlIt != list.end())
				return (*controlIt).get();
		}
		return nullptr;
	};


	TouchControl *InputManager::getControlByName(const std::string &_setName, const std::string &_controlName)
	{
		TouchControlSetsIterator it = controlSets.find(_setName);
		assert(it != controlSets.end() && "specified set doesn't exist");

		TouchControlList &list = (*it).second;
		auto controlIt = std::find_if(list.begin(), list.end(), [_controlName] (TouchControl* _control) { return !_controlName.compare(_control->getName()); });

		return controlIt != list.end() ? (*controlIt).get() : nullptr;
	};


	void InputManager::removeControl(const std::string &_setName, std::string &_controlName)
	{
		auto it = controlSets.find(_setName);
		if (it == controlSets.end())
			return;

		TouchControlList &list = (*it).second;
		auto it2 = std::find_if(list.begin(), list.end(), [_controlName] (TouchControl* _control) { return !_controlName.compare(_control->getName()); });

		if (it2 != list.end())
		{
			removeFromActivePointers((*it2).get());
			std::swap(*it2, list.back());
			list.pop_back();
		}
	};


	void InputManager::removeControlSet(const std::string &_setName)
	{
		auto setIt = controlSets.find(_setName);
		if (setIt != controlSets.end())
		{
			if (currentSetName == _setName)
			{
				currentSetName.clear();
				currentSet = nullptr;
			}

			TouchControlList &list = (*setIt).second;
			for (auto it = list.begin(), itEnd = list.end(); it != itEnd; ++it)
				removeFromActivePointers((*it).get());
		}

		controlSets.erase(_setName);
	};


	void InputManager::removeAllControls()
	{
		controlSets.clear();
		activePointers.clear();
	};


	void InputManager::activateControlSet(const std::string &_setName)
	{
		currentSetName = _setName;
		auto it = controlSets.find(_setName);
		currentSet = (it != controlSets.end()) ? &(*it).second : nullptr;
		active = true;
	};


	void InputManager::deactivate()
	{
		active = false;
		currentSetName = emptySetName;
		currentSet = nullptr;
	};


	TouchControl *InputManager::getAffectedControl(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		if (!currentSet)
			return nullptr;

		for (auto it = currentSet->begin(), itEnd = currentSet->end(); it != itEnd; ++it)
			if ((*it)->containsPointer(_pointerPosition))
				return (*it).get();

		return nullptr;
	};


	int InputManager::onTouchEvent(AInputEvent* _event)
	{
		int32_t action = AMotionEvent_getAction(_event);
		if (action < 0 || !active) return action;

		int flags = action & AMOTION_EVENT_ACTION_MASK;
		size_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

		int32_t pointerId = AMotionEvent_getPointerId(_event, pointerIndex);
		if (pointerId < 0) return pointerId;

		Vector2 pointerPosition = {
			AMotionEvent_getX(_event, pointerIndex),
			AMotionEvent_getY(_event, pointerIndex)
		};

		PointerPosition position = createPointerPosition(pointerPosition.x, pointerPosition.y);


		TouchControl *affectedPointer = getAffectedControl(pointerId, position);
		if (!affectedPointer)
		{
			// remove active pointer if moved out
			auto it = activePointers.find(pointerId);
			if (it != activePointers.end())
			{
				(*it).second.affectedControl->onPointerMoveOut((*it).second.pointerId, position);
				activePointers.erase(it);
			}

			return 0;
		}


		switch (flags)
		{
			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_POINTER_UP:
			{
				auto it = activePointers.find(pointerId);
				PointerData &pDataRes = (*it).second;

				pDataRes.affectedControl->onPointerUp(pDataRes.pointerId, position);
				activePointers.erase(it);

				break;
			}

			case AMOTION_EVENT_ACTION_MOVE:
			{
				auto it = activePointers.find(pointerId);
				if (it != activePointers.end())
				{
					PointerData &pDataRes = (*it).second;
					pDataRes.position = pointerPosition;

					if (pDataRes.affectedControl == affectedPointer)
					{
						pDataRes.affectedControl->onPointerMove(pDataRes.pointerId, position);
					}
					else
					{
						pDataRes.affectedControl->onPointerMoveOut(pDataRes.pointerId, position);

						pDataRes.affectedControl = affectedPointer;
						pDataRes.affectedControl->onPointerMoveIn(pDataRes.pointerId, position);
					}
				}
				else
				{
					PointerData pData = { pointerId, pointerPosition, affectedPointer };
					activePointers.emplace(pointerId, pData);
					pData.affectedControl->onPointerMoveIn(pData.pointerId, position);
				}

				break;
			}

			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
			{
				PointerData pData = { pointerId, pointerPosition, affectedPointer };

				auto res = activePointers.emplace(pointerId, pData);
				PointerData &pDataRes = (*res.first).second;

				pDataRes.affectedControl->onPointerDown(pDataRes.pointerId, position);

				break;
			}
		}

		return 0;
	};

}
