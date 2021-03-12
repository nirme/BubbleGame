#pragma once

#include "_2d/MovableObject.h"
#include "_2d/Camera.h"
#include "TouchControls.h"


using namespace core;
using namespace core::_2d;

class ObjectAttachedTouchButton : public TouchButton
{
protected:
	MovableObject *connectedObject;
	Camera *usedCamera;

public:
	ObjectAttachedTouchButton(const std::string& _name,
					  MovableObject *_object,
					  std::unique_ptr<_2d::Shape> _objectShape,
					  Camera *_usedCamera);

	virtual bool containsPointer(const PointerPosition &_pointerPosition) const;
	void changeObject(MovableObject *_object);
	void changeCamera(Camera *_usedCamera);

};

