#include "PhysicsSystem.h"
#include "RigidObject.h"


template<>
core::_2d::PhysicsSystem* Singleton<core::_2d::PhysicsSystem>::impl = nullptr;

namespace core
{
	namespace _2d
	{
		PhysicsSystem::CollidableTypeIndex PhysicsSystem::freeTypeIndex = 1;


		PhysicsSystem::CollidableObjectPair::CollidableObjectPair(CollidableTypeIndex _typeId1, CollidableTypeIndex _typeId2) :
			typeId1(_typeId1),
			typeId2(_typeId2),
			collisionId((unsigned int)_typeId1 * _typeId2 * 2 -_typeId1 - _typeId2) // assuming typeid never go above 46340
		{};


		bool PhysicsSystem::CollidableObjectPair::operator==(const CollidableObjectPair &_rhs) const
		{
			return typeId1 == _rhs.typeId1 && typeId2 == _rhs.typeId2;
		};


		size_t PhysicsSystem::CollidableObjectPair::Hash::operator()(CollidableObjectPair const& _pair) const noexcept
		{
			return _pair.collisionId;
		};


		PhysicsSystem::CollidablePairDescriptor::CollidablePairDescriptor(bool _reflect, CollisionListener *_listener) :
			reflect(_reflect),
			listener(_listener)
		{};


		PhysicsSystem::CollidablePairDescriptor::CollidablePairDescriptor(CollidablePairDescriptor &&_rhs) : reflect(_rhs.reflect)
		{
			listener.swap(_rhs.listener);
		};


		PhysicsSystem::CollidablePairDescriptor &PhysicsSystem::CollidablePairDescriptor::operator= (CollidablePairDescriptor &&_rhs)
		{
			reflect = _rhs.reflect;
			listener.swap(_rhs.listener);
			return *this;
		};


		PhysicsSystem::OverlappingObjectPair::OverlappingObjectPair(RigidObject *_object1, RigidObject *_object2) :
			object1(_object1),
			object2(_object2)
		{};


		bool PhysicsSystem::OverlappingObjectPair::operator==(const OverlappingObjectPair &_rhs) const
		{
			return object1 == _rhs.object1 && object2 == _rhs.object2;
		};


		size_t PhysicsSystem::OverlappingObjectPair::Hash::operator()(OverlappingObjectPair const& _pair) const noexcept
		{
			return (size_t)(reinterpret_cast<std::uintptr_t>(_pair.object1) ^ reinterpret_cast<std::uintptr_t>(_pair.object2));
		};


		void PhysicsSystem::registerAffector(std::string _name, PhysicsAffectorPtr _affector)
		{
			std::pair<AffectorsMap::iterator, bool> emplaceResult = activeAffectors.emplace(_name, _affector);
			if (!emplaceResult.second)
				(*emplaceResult.first).second = _affector;

			for (unsigned int i = 0, iEnd = registeredObjects.size(); i < iEnd; ++i)
				registeredObjects[i]->setAffector(_name, _affector);
		};


		void PhysicsSystem::unregisterAffector(std::string _name)
		{
			for (unsigned int i = 0, iEnd = registeredObjects.size(); i < iEnd; ++i)
				registeredObjects[i]->removeAffector(_name);

			activeAffectors.erase(_name);
		};


		PhysicsAffectorPtr PhysicsSystem::getAffector(std::string _name)
		{
			auto it = activeAffectors.find(_name);
			return it != activeAffectors.end() ? (*it).second : nullptr;
		};


		void PhysicsSystem::registerCollidablePair(const std::string &_collidableType1, const std::string &_collidableType2, bool _reflect, CollisionListener *_listener)
		{
			CollidableTypeIndex index1 = getObjectTypeIndex(_collidableType1);
			CollidableTypeIndex index2 = getObjectTypeIndex(_collidableType2);
			CollidableObjectsPairs::iterator findResult;

			if ((findResult = collidablePairs.find(CollidableObjectPair(index1, index2))) != collidablePairs.end())
				(*findResult).second = CollidablePairDescriptor(_reflect, _listener);
			else if ((findResult = collidablePairs.find(CollidableObjectPair(index2, index1))) != collidablePairs.end())
				(*findResult).second = CollidablePairDescriptor(_reflect, _listener);
			else
				collidablePairs.emplace(CollidableObjectPair(index1, index2), CollidablePairDescriptor(_reflect, _listener));
		};


		void PhysicsSystem::unregisterCollidablePair(const std::string &_collidableType1, const std::string &_collidableType2)
		{
			CollidableTypeIndex index1 = getObjectTypeIndex(_collidableType1);
			CollidableTypeIndex index2 = getObjectTypeIndex(_collidableType2);

			collidablePairs.erase(CollidableObjectPair(index1, index2));
			collidablePairs.erase(CollidableObjectPair(index2, index1));
		};


		void PhysicsSystem::progressTime(float _frameTime)
		{
			if (!enabled)
				return;

			// move all the objects first
			for (unsigned int i = 0, iEnd = registeredObjects.size(); i < iEnd; ++i)
			{
				// update all shapes by requesting current aabb
				if (registeredObjects[i]->isEnabled())
				{
					registeredObjects[i]->progress(_frameTime);
				}
			}

			OverlappingObjectsList currentlyOverlappingObjects;

			for (CollidableObjectsPairs::iterator it = collidablePairs.begin(), itEnd = collidablePairs.end(); it != itEnd; ++it)
			{
				auto findResult = registeredObjectsMap.find((*it).first.typeId1);
				if (findResult == registeredObjectsMap.end()) continue;
				std::vector<RigidObject*> &objList1 = (*findResult).second;

				findResult = registeredObjectsMap.find((*it).first.typeId2);
				if (findResult == registeredObjectsMap.end()) continue;
				std::vector<RigidObject*> &objList2 = (*findResult).second;

				// for convenience
				typedef RigidObject::ShapesList CShapesList;

				// not many object will be used at once so just go through all
				for (unsigned int i = 0, iEnd = objList1.size(); i < iEnd; ++i)
				{
					for (unsigned int j = (objList1 != objList2 ? 0 : (i+1)), jEnd = objList2.size(); j < jEnd; ++j)
					{
						if ((objList1[i]->isStatic() && objList2[j]->isStatic()) || !objList1[i]->isEnabled() || !objList2[j]->isEnabled())
							continue;

						if (AxisAlignedBox::isOverlapping(objList1[i]->getBoundingBox(), objList2[j]->getBoundingBox()))
						{
							const CShapesList &obj1Shapes = objList1[i]->getCurrentFrameShapes();
							const CShapesList &obj2Shapes = objList2[j]->getCurrentFrameShapes();
							const CShapesList &obj1ShapesPrev = objList1[i]->getPreviousFrameShapes();
							const CShapesList &obj2ShapesPrev = objList2[j]->getPreviousFrameShapes();

							float collisionTime = std::numeric_limits<float>::infinity();
							float distanceAfterMovement = std::numeric_limits<float>::infinity();
							float distAfter, distBefore, tmp;
							unsigned int obj1ShapeIndex, obj2ShapeIndex;

							for (unsigned int iObj1 = 0, iObj1End = obj1Shapes.size(); iObj1 < iObj1End; ++iObj1)
							{
								for (unsigned int iObj2 = 0, iObj2End = obj2Shapes.size(); iObj2 < iObj2End; ++iObj2)
								{
									distAfter = obj1Shapes[iObj1]->distance(obj2Shapes[iObj2].get());
									if (distAfter < 0.0f)
									{
										distBefore = obj1ShapesPrev[iObj1]->distance(obj2ShapesPrev[iObj2].get());
										tmp = distBefore /(distBefore - distAfter);

										// find shapes that hit each other first
										// assuming linear motion between frames
										if (collisionTime > tmp)
										{
											collisionTime = tmp;
											obj1ShapeIndex = iObj1;
											obj2ShapeIndex = iObj2;
										}
									}
								}
							}

							if (collisionTime == std::numeric_limits<float>::infinity())
								continue;


							if (collisionTime < 0.0f)
								collisionTime = 0.0f;


							if ((*it).second.reflect)
							{
								// one is static
								if (objList1[i]->isStatic() || objList2[j]->isStatic())
								{
									RigidObject *staticObject, *bouncingObject;
									Shape *staticShape, *bouncingShape;
									if (objList1[i]->isStatic())
									{
										staticObject = objList1[i];
										bouncingObject = objList2[j];
										staticShape = obj1Shapes[obj1ShapeIndex].get();
										bouncingShape = obj2Shapes[obj2ShapeIndex].get();
									}
									else
									{
										staticObject = objList2[j];
										bouncingObject = objList1[i];
										staticShape = obj2Shapes[obj2ShapeIndex].get();
										bouncingShape = obj1Shapes[obj1ShapeIndex].get();
									}

									Vector2 reflectionNormal = staticShape->separatingAxisNormal(bouncingShape);

									// rewind transformation
									bouncingObject->rewindTransformation();

									// progress to first collision
									bouncingObject->progressTransformation(_frameTime * collisionTime);


									// only level walls will be static so just bounce as reflections

									// get reflected direction of sum of both objects dir
									Vector2 reflectedDirection = bouncingObject->getDirectionVector() + staticObject->getDirectionVector();
									if (dotProduct(reflectedDirection, reflectedDirection) < EPSILON)
										reflectedDirection = -reflectionNormal;
									reflectedDirection = -reflectAcrossNormal(reflectedDirection, reflectionNormal);

									// no bounciness factor for this project required
									// affect by bounciness
									//reflectedDirection *= bouncingObject->getBounceFactor() * staticObject->getBounceFactor();


									// update with new direction and complete movement
									bouncingObject->setDirectionVector(reflectedDirection);
									bouncingObject->progressPartialTransformation(_frameTime - (_frameTime * collisionTime));
									bouncingObject->updateTransformation();
								}
								else // both are movable
								{
									Vector2 reflectionNormal = obj1Shapes[obj1ShapeIndex]->separatingAxisNormal(obj2Shapes[obj2ShapeIndex].get());

									// rewind transformation
									objList1[i]->rewindTransformation();
									objList2[j]->rewindTransformation();

									// progress to first collision
									float beforeFrameTime = _frameTime * collisionTime;
									objList1[i]->progressTransformation(beforeFrameTime);
									objList2[j]->progressTransformation(beforeFrameTime);


									bool flatReflectionFlag = false;
									bool flatReflectionX;
									if (std::abs(reflectionNormal.x) > transformedNormalX)
									{
										if (reflectionNormal.x < 0.0f)
											reflectionNormal = -1.0f;

										Vector2 direction = objList1[i]->getDirectionVector();
										if (reflectionNormal.x * direction.x > 0.0f)
										{
											direction.x = -direction.x;
											objList1[i]->setDirectionVector(direction);
										}

										direction = objList2[j]->getDirectionVector();
										if (reflectionNormal.x * direction.x < 0.0f)
										{
											direction.x = -direction.x;
											objList2[j]->setDirectionVector(direction);
										}
									}
									else if (std::abs(reflectionNormal.y) > transformedNormalX)
									{
										if (reflectionNormal.y < 0.0f)
											reflectionNormal = -1.0f;

										Vector2 direction = objList1[i]->getDirectionVector();
										if (reflectionNormal.y * direction.y > 0.0f)
										{
											direction.y = -direction.y;
											objList1[i]->setDirectionVector(direction);
										}

										direction = objList2[j]->getDirectionVector();
										if (reflectionNormal.y * direction.y < 0.0f)
										{
											direction.y = -direction.y;
											objList2[j]->setDirectionVector(direction);
										}
									}
									else
									{
										if (dotProduct(reflectionNormal, objList1[i]->getDirectionVector()) > 0.0f)
											objList1[i]->setDirectionVector(-objList1[i]->getDirectionVector());

										if (dotProduct(reflectionNormal, objList2[j]->getDirectionVector()) < 0.0f)
											objList2[j]->setDirectionVector(-objList2[j]->getDirectionVector());
									}

									/*
									// get reflected direction of sum of both objects dir
									float bounceFactor = objList1[i]->getBounceFactor() * objList2[j]->getBounceFactor();

									//bool noDirectionFlag = dotProduct(objList1[i]->getDirectionVector(), objList2[j]->getDirectionVector()) < EPSILON;
									Vector2 reflectedDirection1 = -reflectAcrossNormal(objList1[i]->getDirectionVector(), reflectionNormal) * bounceFactor;
									Vector2 reflectedDirection2 = -reflectAcrossNormal(objList2[j]->getDirectionVector(), -reflectionNormal) * bounceFactor;

									objList1[i]->setDirectionVector(reflectedDirection1);
									objList2[j]->setDirectionVector(reflectedDirection2);
									*/

									objList1[i]->progressPartialTransformation(_frameTime - beforeFrameTime);
									objList2[j]->progressPartialTransformation(_frameTime - beforeFrameTime);

									objList1[i]->updateTransformation();
									objList2[j]->updateTransformation();
								}


								if ((*it).second.listener)
									(*it).second.listener->onCollisionDetected(objList1[i], objList2[j]);

								objList1[i]->collisionDetected(objList2[j]);
								objList2[j]->collisionDetected(objList1[i]);
							}
							else // not reflected
							{
								auto resultIt = currentlyOverlappingObjects.emplace(
										OverlappingObjectPair(objList1[i], objList2[j]),
										CollidableObjectPair((*it).first.typeId1, (*it).first.typeId2)
										).first;

								if (!overlappingObjects.erase(OverlappingObjectPair(objList1[i], objList2[j])))
								{
									if ((*it).second.listener)
										(*it).second.listener->onCollisionDetected(objList1[i], objList2[j]);

									objList1[i]->collisionDetected(objList2[j]);
									objList2[j]->collisionDetected(objList1[i]);
								}
							}
						}
					}
				}
			} // end for

			// left with only non overlapping objects now
			if (overlappingObjects.size())
			{
				for (OverlappingObjectsList::iterator it = overlappingObjects.begin(), itEnd = overlappingObjects.end(); it != itEnd; ++it)
				{
					auto pairIt = collidablePairs.find((*it).second);
					if (pairIt != collidablePairs.end())
					{
						if ((*pairIt).second.listener)
							(*pairIt).second.listener->onCollisionEnded((*it).first.object1, (*it).first.object2);

						(*it).first.object1->collisionEnded((*it).first.object2);
						(*it).first.object2->collisionEnded((*it).first.object1);
					}
				}
			}

			currentlyOverlappingObjects.swap(overlappingObjects);
		};


		void PhysicsSystem::unregisterObject(RigidObject *_object)
		{
			CollidableTypeIndex typeIndex = getObjectTypeIndex(_object->getCollidableObjectType());
			RegisteredCollidableObjectList::iterator objIt = std::find(registeredObjects.begin(), registeredObjects.end(), _object);

			assert(objIt != registeredObjects.end() && "cannot unregister non existing object");

			for (AffectorsMap::iterator it = activeAffectors.begin(), itEnd; it != itEnd; ++it)
				_object->removeAffector((*it).first);

			std::swap(*objIt, registeredObjects.back());
			registeredObjects.pop_back();

			CollidableObjectMap::iterator objectVectorIt = registeredObjectsMap.find(typeIndex);
			if (objectVectorIt != registeredObjectsMap.end())
			{
				std::vector<RigidObject*> &objectVector = objectVectorIt->second;

				// remove entire vector if removing last object
				if (objectVector.size() == 1 && objectVector.front() == _object)
				{
					registeredObjectsMap.erase(typeIndex);
				}
				else
				{
					auto it = std::find(objectVector.begin(), objectVector.end(), _object);
					assert(it != objectVector.end() && "can't find the object to remove");

					std::swap(*it, objectVector.back());
					objectVector.pop_back();
				}
			}
		};


		void PhysicsSystem::registerObject(RigidObject *_object)
		{
			assert(std::find(registeredObjects.begin(), registeredObjects.end(), _object) == registeredObjects.end()
					&& "object cannot be registered 2 times");

			registeredObjects.push_back(_object);

			CollidableTypeIndex typeIndex = getObjectTypeIndex(_object->getCollidableObjectType());
			CollidableObjectMap::iterator objectVectorIt = registeredObjectsMap.find(typeIndex);
			if (objectVectorIt != registeredObjectsMap.end())
			{
				(*objectVectorIt).second.push_back(_object);
			}
			else
			{
				registeredObjectsMap.emplace(std::piecewise_construct, std::make_tuple(typeIndex), std::make_tuple(1, _object));
			}

			for (AffectorsMap::iterator it = activeAffectors.begin(), itEnd; it != itEnd; ++it)
				_object->setAffector((*it).first, (*it).second);
		};


	}
}
