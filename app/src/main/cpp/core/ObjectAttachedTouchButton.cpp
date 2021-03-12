#include "ObjectAttachedTouchButton.h"


ObjectAttachedTouchButton::ObjectAttachedTouchButton(const std::string& _name,
				  MovableObject *_object,
				  std::unique_ptr<_2d::Shape> _objectShape,
				  Camera *_usedCamera) :
		TouchButton(_name, std::move(_objectShape)),
		connectedObject(_object),
		usedCamera(_usedCamera)
{};


bool ObjectAttachedTouchButton::containsPointer(const PointerPosition &_pointerPosition) const
{
	Vector2 objectSpacePointer =
			inverse(usedCamera->getViewMatrix() * connectedObject->getWorldTransform()) *
			Vector2(_pointerPosition.projSpaceX, _pointerPosition.projSpaceY);

	return buttonShape->contains(objectSpacePointer);
};


void ObjectAttachedTouchButton::changeObject(MovableObject *_object)
{
	connectedObject = _object;
};


void ObjectAttachedTouchButton::changeCamera(Camera *_usedCamera)
{
	usedCamera = _usedCamera;
};

