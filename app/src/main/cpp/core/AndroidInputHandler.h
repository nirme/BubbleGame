#pragma once

#include <memory>
#include <map>

namespace core
{

	class AndroidInputHandler
	{
	protected:

		int screenWidh;
		int screenHeight;

		struct TouchPointer
		{
			int pointerIndex;

			float startingPositionX;
			float startingPositionY;

			float positionX;
			float positionY;

			TouchPointer() :
				pointerIndex(0), 
				startingPositionX(0.0f), 
				startingPositionY(0.0f), 
				positionX(0.0f), 
				positionY(0.0f)
			{};

			TouchPointer(int _pointerIndex, float _positionX, float _positionY) :
				pointerIndex(_pointerIndex), 
				startingPositionX(_positionX), 
				startingPositionY(_positionY), 
				positionX(_positionX), 
				positionY(_positionY)
			{};
		};

		typedef std::map<int, TouchPointer> TouchPointersMap;
		TouchPointersMap activeTouchPoints;


	private:

		void fireTouchPointerUpEvent(const TouchPointer& _pointer)
		{};

		void fireTouchPointerMoveEvent(const TouchPointer& _pointer)
		{};

		void fireTouchPointerDownEvent(const TouchPointer& _pointer)
		{};


	public:

		AndroidInputHandler()
		{};


		int onTouchEvent(AInputEvent* event)
		{
			int32_t action = AMotionEvent_getAction(event);
			if (action < 0) return action;

			size_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

			int32_t pid = AMotionEvent_getPointerId(event, index);
			if (pid < 0) return pid;

			int flags = action & AMOTION_EVENT_ACTION_MASK;


			AMotionEvent_getX(event, index);
			AMotionEvent_getY(event, index);

			//activeTouchPoints
			switch (flags)
			{
				case AMOTION_EVENT_ACTION_UP:
				case AMOTION_EVENT_ACTION_POINTER_UP:
				{
					auto it = activeTouchPoints.find(pid);
					if (it != activeTouchPoints.end())
					{
						fireTouchPointerUpEvent((*it).second);
						activeTouchPoints.erase(pid);
					}
					break;
				}

				case AMOTION_EVENT_ACTION_MOVE:
				{
					auto it = activeTouchPoints.find(pid);
					if (it != activeTouchPoints.end())
					{
						(*it).second.positionX = AMotionEvent_getX(event, index);
						(*it).second.positionY = AMotionEvent_getY(event, index);

						fireTouchPointerMoveEvent((*it).second);
						break;
					}
					// pointer not in map yet
				}

				case AMOTION_EVENT_ACTION_DOWN:
				case AMOTION_EVENT_ACTION_POINTER_DOWN:
				{
					auto it = activeTouchPoints.emplace(pid, TouchPointer(pid, AMotionEvent_getX(event, index), AMotionEvent_getY(event, index)));
					fireTouchPointerDownEvent((*it.first).second);
					break;
				}
			}

			return 0;

		};

	};

	typedef std::shared_ptr<AndroidInputHandler> AndroidInputHandlerPtr;
}
