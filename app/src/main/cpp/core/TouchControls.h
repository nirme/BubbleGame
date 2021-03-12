#pragma once

#include <memory>
#include <unordered_set>
#include "TouchControl.h"
#include "_2d/shapes/Shape.h"

namespace core
{

	class TouchButton : public TouchControl
	{
	public:
		class Listener : public TouchControl::Listener
		{
		public:
			virtual void onButtonDown(){};
			virtual void onButtonUp(){};
			virtual void onButtonClicked(){};
		};

	protected:
		std::unique_ptr<_2d::Shape> buttonShape;
		std::unordered_set<int32_t> pointers;

		inline Listener *getListener()
		{
			Listener * ptr = dynamic_cast<Listener*>(buttonListener);
			return ptr;
		};

	public:
		TouchButton(const std::string& _name, std::unique_ptr<_2d::Shape> _controlShape);
		virtual bool containsPointer(const PointerPosition &_pointerPosition) const;
		void onPointerDown(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerUp(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerMove(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerMoveIn(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerMoveOut(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void reset(int32_t _pointerId);
	};


	class TouchArea : public TouchControl
	{
	public:
		class Listener : public TouchControl::Listener
		{
		public:
			virtual void onPointerOnArea() = 0;
			virtual void onPointerOffArea() = 0;
		};

	protected:

		std::unique_ptr<_2d::Shape> areaShape;
		std::unordered_set<int32_t> pointers;

		inline Listener *getListener()
		{
			Listener * ptr = dynamic_cast<Listener*>(buttonListener);
			return ptr;
		};

	public:
		TouchArea(const std::string& _name, std::unique_ptr<_2d::Shape> _controlShape);
		virtual bool containsPointer(const PointerPosition &_pointerPosition) const;
		void onPointerDown(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerUp(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerMove(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerMoveIn(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void onPointerMoveOut(int32_t _pointerId, const PointerPosition &_pointerPosition);
		void reset(int32_t _pointerId);
	};

}
