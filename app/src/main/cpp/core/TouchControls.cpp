#include "TouchControls.h"


namespace core
{

	TouchButton::TouchButton(_2d::Shape *_controlShape) :
		buttonShape(_controlShape)
	{};


	void TouchButton::onPointerDown(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onButtonDown();

		pointers.insert(_pointerId);
	};


	bool TouchButton::containsPointer(int32_t _pointerId, const PointerPosition &_pointerPosition) const
	{
		Vector2 point(_pointerPosition.denormProjSpaceX, _pointerPosition.projSpaceY);
		return buttonShape->contains(point);
	};


	void TouchButton::onPointerUp(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		pointers.erase(_pointerId);

		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
		{
			ptr->onButtonUp();
			ptr->onButtonClicked();
		}
	};


	void TouchButton::onPointerMove(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{};


	void TouchButton::onPointerMoveIn(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onButtonDown();

		pointers.insert(_pointerId);
	};


	void TouchButton::onPointerMoveOut(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		pointers.erase(_pointerId);

		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onButtonUp();
	};


	void TouchButton::reset(int32_t _pointerId)
	{
		unsigned int pointerCount = pointers.size();
		if (_pointerId)		pointers.erase(_pointerId);
		else				pointers.clear();

		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty() && pointerCount)
			ptr->onButtonUp();
	};



	TouchArea::TouchArea(_2d::Shape *_controlShape) :
		areaShape(_controlShape)
	{};


	bool TouchArea::containsPointer(int32_t _pointerId, const PointerPosition &_pointerPosition) const
	{
		Vector2 point(_pointerPosition.projSpaceX, _pointerPosition.projSpaceY);
		return areaShape->contains(point);
	};


	void TouchArea::onPointerDown(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onPointerOnArea();

		pointers.insert(_pointerId);
	};


	void TouchArea::onPointerUp(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		pointers.erase(_pointerId);

		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onPointerOffArea();
	};


	void TouchArea::onPointerMove(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{};


	void TouchArea::onPointerMoveIn(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onPointerOnArea();

		pointers.insert(_pointerId);
	};


	void TouchArea::onPointerMoveOut(int32_t _pointerId, const PointerPosition &_pointerPosition)
	{
		pointers.erase(_pointerId);

		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty())
			ptr->onPointerOffArea();
	};


	void TouchArea::reset(int32_t _pointerId)
	{
		unsigned int pointerCount = pointers.size();
		if (_pointerId)		pointers.erase(_pointerId);
		else				pointers.clear();

		Listener *ptr = dynamic_cast<Listener*>(buttonListener);
		if (ptr && pointers.empty() && pointerCount)
			ptr->onPointerOffArea();
	};

}
