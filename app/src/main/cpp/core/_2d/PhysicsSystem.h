#pragma once

#include <algorithm>
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "../SingletonTemplate.h"
#include "AxisAlignedBox.h"
#include "MovableObject.h"
#include "shapes/ShapeMath.h"

#include "PhysicsAffector.h"
#include "RigidObject.h"
#include "../ControllerManager.h"
#include "../Controller.h"


namespace core
{
	namespace _2d
	{

		class RigidObject;

		class PhysicsSystem : public Singleton<PhysicsSystem>
		{
			// if within this range the bounce like from vertical wall
			static constexpr unsigned int maxRotationFromX = 30;
			static constexpr float transformedNormalX = 0.86602540378; // std::cos((maxRotationFromX/180.0f) * M_PI);
			//static constexpr float transformedNormalX = 0.70710678118;

		public:
			class CollisionListener
			{
			public:
				virtual void onCollisionDetected(RigidObject *_object1, RigidObject *_object2) = 0;
				virtual void onCollisionEnded(RigidObject *_object1, RigidObject *_object2) = 0;
				virtual ~CollisionListener(){};
			};

		protected:

			bool enabled;

			typedef std::unordered_map<std::string, PhysicsAffectorPtr> AffectorsMap;
			AffectorsMap activeAffectors;

			typedef unsigned short CollidableTypeIndex;

			static CollidableTypeIndex freeTypeIndex;

			typedef std::unordered_map<std::string, CollidableTypeIndex> CollidableObjectsTypeMap;
			CollidableObjectsTypeMap objectTypeMap;

			struct CollidableObjectPair
			{
				CollidableTypeIndex typeId1;
				CollidableTypeIndex typeId2;
				unsigned int collisionId;

				CollidableObjectPair(CollidableTypeIndex _typeId1, CollidableTypeIndex _typeId2);
				bool operator==(const CollidableObjectPair &_rhs) const;

				struct Hash
				{
					size_t operator()(CollidableObjectPair const& _pair) const noexcept;
				};
			};


			struct CollidablePairDescriptor
			{
				bool reflect;
				std::unique_ptr<CollisionListener> listener;

				CollidablePairDescriptor(bool _reflect, CollisionListener *_listener);
				CollidablePairDescriptor(CollidablePairDescriptor &&_rhs);
				CollidablePairDescriptor &operator= (CollidablePairDescriptor &&_rhs);
			};

			typedef std::unordered_map<CollidableObjectPair, CollidablePairDescriptor, CollidableObjectPair::Hash> CollidableObjectsPairs;
			CollidableObjectsPairs collidablePairs;

			typedef std::vector<RigidObject*> RegisteredCollidableObjectList;
			RegisteredCollidableObjectList registeredObjects;

			typedef std::unordered_map<CollidableTypeIndex, std::vector<RigidObject*>> CollidableObjectMap;
			CollidableObjectMap registeredObjectsMap;



			struct OverlappingObjectPair
			{
				RigidObject *object1, *object2;

				OverlappingObjectPair(RigidObject *_object1, RigidObject *_object2);
				bool operator==(const OverlappingObjectPair &_rhs) const;

				struct Hash
				{
					size_t operator()(OverlappingObjectPair const& _pair) const noexcept;
				};
			};

			typedef std::unordered_map<OverlappingObjectPair, CollidableObjectPair, OverlappingObjectPair::Hash> OverlappingObjectsList;
			OverlappingObjectsList overlappingObjects;



			inline CollidableTypeIndex getObjectTypeIndex(const std::string &_objectTypeName)
			{
				CollidableObjectsTypeMap::iterator indexIt = objectTypeMap.find(_objectTypeName);
				if (indexIt != objectTypeMap.end())
					return (*indexIt).second;

				auto emplaceResult = objectTypeMap.emplace(_objectTypeName, freeTypeIndex);
				++freeTypeIndex;

				return (*emplaceResult.first).second;
			};


		public:

			void setEnabled(bool _enabled)
			{
				enabled = _enabled;
			};

			void registerAffector(std::string _name, PhysicsAffectorPtr _affector);
			void unregisterAffector(std::string _name);
			PhysicsAffectorPtr getAffector(std::string _name);

			void registerCollidablePair(const std::string &_collidableType1, const std::string &_collidableType2, bool _reflect = true, CollisionListener *_listener = nullptr);
			void unregisterCollidablePair(const std::string &_collidableType1, const std::string &_collidableType2);

			void progressTime(float _frameTime);

			void unregisterObject(RigidObject *_object);
			void registerObject(RigidObject *_object);

		};
	}
}
