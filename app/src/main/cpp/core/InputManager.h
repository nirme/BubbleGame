#pragma once

#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>

#include "InputHandler.h"
#include "Vector2.h"
#include "TouchControl.h"

#include "SingletonTemplate.h"



// android touchscreen coords:
// (0,0)	 (maxX,0)
// (0,maxY)	 (maxX,maxY)

// proj space
// (-1, 1)	 (1, 1)
// (-1,-1)	 (1,-1)

// denorm proj space
// (-1*(w/h), 1)	 (1*(w/h), 1)
// (-1*(w/h),-1)	 (1*(w/h),-1)


namespace core
{
	class InputManager : public InputHandler
	{
	private:
		struct ScreenPosition
		{
			unsigned short x;
			unsigned short y;
		};

		ScreenPosition screenSize;
		Vector2 inverseScreenSize;

		typedef std::vector<TouchControl*> TouchControlList;
		typedef std::unordered_map<std::string, std::unique_ptr<TouchControlList>> TouchControlSets;
		typedef TouchControlSets::iterator TouchControlSetsIterator;

		TouchControlSets controlSets;
		std::string currentSetName;
		TouchControlList *currentSet;

		struct PointerData
		{
			int32_t pointerId;
			Vector2 position;
			TouchControl* affectedControl;
		};

		typedef std::unordered_map<int32_t, PointerData> ActivePointerList;
		ActivePointerList activePointers;



		PointerPosition createPointerPosition(float _x, float _y);
		void removeFromActivePointers(TouchControl *_control);

	public:

		InputManager();

		void setScreenSize(unsigned short _width, unsigned short _height);

		void registerControl(const std::string &_setName, TouchControl *_control);
		void removeControl(const std::string &_setName, TouchControl *_control);
		void removeControlSet(const std::string &_setName);
		void removeAllControls();

		void activateControlSet(const std::string &_setName);

		TouchControl *getAffectedControl(int32_t _pointerId, const PointerPosition &_pointerPosition);
		int onTouchEvent(AInputEvent* _event);

	};

}
