#pragma once
#include <android/input.h>

namespace core
{

	class InputHandler
	{
	public:
		virtual int onTouchEvent(AInputEvent* event) = 0;
		//virtual bool onKeyboardEvent(AInputEvent* event) = 0;
	};

}
