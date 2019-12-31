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
		screenSize{ 1, 1 },
		inverseScreenSize{ 1.0f, 1.0f },
		currentSetName(""),
		currentSet(nullptr)
	{};


	void InputManager::setScreenSize(unsigned short _width, unsigned short _height)
	{
		screenSize = { _width, _height };
		inverseScreenSize = Vector2(1.0f / screenSize.x, 1.0f / screenSize.y);
	};


	void InputManager::registerControl(const std::string &_setName, TouchControl *_control)
	{
		assert(_control && "cannot add nullptr as touch control");

		auto res = controlSets.find(_setName);
		if (res == controlSets.end())
		{
			auto insertRes = controlSets.insert(std::make_pair(_setName, nullptr));
			res = insertRes.first;
            (*res).second = std::unique_ptr<TouchControlList>(new TouchControlList());
		}
		TouchControlList *list = (*res).second.get();

		auto it = std::find(list->begin(), list->end(), _control);
		if (it == list->end())
			list->push_back(_control);
	};


	void InputManager::removeControl(const std::string &_setName, TouchControl *_control)
	{
		assert(_control && "cannot remove nullptr as touch control");

		auto it = controlSets.find(_setName);
		if (it != controlSets.end())
		{
			auto it2 = std::find((*it).second->begin(), (*it).second->end(), _control);
			if (it2 != (*it).second->end())
			{
				removeFromActivePointers(_control);

				std::swap(*it2, (*it).second->back());
				(*it).second->pop_back();
			}
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

			TouchControlList *list = (*setIt).second.get();
			for (auto it = list->begin(), itEnd = list->end(); it != itEnd; ++it)
				removeFromActivePointers(*it);
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
		currentSet = (it != controlSets.end()) ? (*it).second.get() : nullptr;
	};


	TouchControl *InputManager::getAffectedControl(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		if (!currentSet)
			return nullptr;

		for (auto it = currentSet->begin(), itEnd = currentSet->end(); it != itEnd; ++it)
			if ((*it)->containsPointer(_pointerId, _pointerPosition))
				return (*it);

		return nullptr;
	};


	int InputManager::onTouchEvent(AInputEvent* _event)
	{
		int32_t action = AMotionEvent_getAction(_event);
		if (action < 0) return action;

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
