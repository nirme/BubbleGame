#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include "ViewPort.h"
#include "SceneNode.h"
#include "RenderQueue.h"
#include "../ShadingParamsPassthru.h"
#include "../RenderSystem.h"
#include "../GraphicBuffer.h"

#include "ObjectFactory.h"



namespace core
{
	namespace _2d
	{
		class Camera;
		class RenderQueue;


		class SceneManager
		{
		protected:

			std::unique_ptr<RenderQueue> renderQueue;
			RenderSystem *renderSystem;
			ShadingParamsPassthru paramsManager;

			std::unique_ptr<ObjectFactory> objectFactory;


			
			std::unique_ptr<Camera> currentCamera;
			std::unique_ptr<ViewPort> currentViewport;

			std::unique_ptr<SceneNode> sceneRoot;


			typedef std::vector<SceneNode*> SceneNodeList;
			typedef std::unordered_map<std::string, SceneNode*> SceneNodeMap;
			SceneNodeList allNodesList;
			SceneNodeMap namedNodes;


			typedef std::vector<MovableObject*> ObjectList;
			ObjectList allObjectsList;

			typedef std::unordered_map<std::string, MovableObject*> ObjectMap;
			ObjectMap namedObjects;


			void addNode(SceneNode* _node);
			void removeNode(SceneNode* _node);

			void addObject(MovableObject* _obj);
			void removeObject(MovableObject* _obj);



			void findVisibleRenderables();

		public:

			SceneManager();

			void setupManager(RenderSystem *_renderSystem, unsigned int _renderTargetWidth, unsigned int _renderTargetHeight, float _sceneScale = 1.0f);

			SceneNode *getNodeByName(const std::string &_name);
			SceneNode *getRootNode();

			SceneNode *createNode(const std::string &_nodeName, ScriptNodePtr _nodeValues);
			void destroyNode(SceneNode *_node);

			SingleSprite *createSingleSprite(const std::string &_name, ScriptNodePtr _nodeValues);




			void renderScene();
		};
	}
}