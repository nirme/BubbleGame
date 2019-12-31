#pragma once

#include "Vector2.h"

namespace core
{

	struct PointerPosition
	{
		float screenPixX, screenPixY;
		float projSpaceX, projSpaceY;
		float denormProjSpaceX; // dwnormalized X coord, based on screen height
	};



	class TouchControl
	{
	public:
		class Listener
		{
		public:
			virtual ~Listener() {};
		};

	protected:
		Listener *buttonListener;

	public:

		virtual ~TouchControl() {};

		virtual bool containsPointer(int32_t _pointerId, const PointerPosition &_pointerPosition) const = 0;

		virtual void onPointerDown(int32_t _pointerId, const PointerPosition &_pointerPosition) = 0;
		virtual void onPointerUp(int32_t _pointerId, const PointerPosition &_pointerPosition) = 0;
		virtual void onPointerMove(int32_t _pointerId, const PointerPosition &_pointerPosition) = 0;

		virtual void onPointerMoveIn(int32_t _pointerId, const PointerPosition &_pointerPosition) = 0;
		virtual void onPointerMoveOut(int32_t _pointerId, const PointerPosition &_pointerPosition) = 0;

		virtual void reset(int32_t _pointerId) = 0;

		void registerListener(Listener *_buttonListener)
		{
			buttonListener = _buttonListener;
		};
	};

}
