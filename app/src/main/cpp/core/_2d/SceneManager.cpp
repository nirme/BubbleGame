#include "SceneManager.h"
#include "../ScriptLoader.h"



namespace core
{
	namespace _2d
	{

		void SceneManager::addNode(SceneNode* _node)
		{
			allNodesList.push_back(_node);
			namedNodes.emplace(_node->getName(), _node);
		};


		void SceneManager::removeNode(SceneNode* _node)
		{
			SceneNodeList::iterator it = std::find(allNodesList.begin(), allNodesList.end(), _node);

			if (it != allNodesList.end())
			{
				std::swap(*it, allNodesList.back());
				allNodesList.pop_back();
			}

			namedNodes.erase(_node->getName());
		};


		void SceneManager::addObject(MovableObject* _obj)
		{
			allObjectsList.push_back(_obj);
			namedObjects.emplace(_obj->getName(), _obj);
		};


		void SceneManager::removeObject(MovableObject* _obj)
		{
			auto it = std::find(allObjectsList.begin(), allObjectsList.end(), _obj);

			if (it != allObjectsList.end())
			{
				std::swap(*it, allObjectsList.back());
				allObjectsList.pop_back();
			}

			namedObjects.erase(_obj->getName());
		};


		void SceneManager::findVisibleRenderables()
		{
			sceneRoot->findVisibleRenderables(currentCamera.get(), renderQueue.get(), &(currentViewport->getFrustrum()));
		};


		SceneManager::SceneManager(std::string _sceneName) :
			sceneName(_sceneName),
			renderSystem(nullptr)
		{};


		SceneManager::~SceneManager()
		{
			clearScene();
			//currentViewport.reset();
			//currentCamera.reset();
			sceneRoot.reset();
		};


		void SceneManager::clearScene()
		{
			destroyObject(currentCamera.release());
			currentViewport.reset();
			sceneRoot->destroyAllChildren();
		};


		void SceneManager::setupManager(RenderSystem *_renderSystem, unsigned int _renderTargetWidth, unsigned int _renderTargetHeight, float _sceneScale)
		{
			assert(!renderSystem && "scene manager already setup");

			renderSystem = _renderSystem;

			// don't use std::make_unique, runtime issues with memory allocation in some cases

			std::unique_ptr<RenderQueue> renderQueueTmp(new RenderQueue());
			renderQueue.swap(renderQueueTmp);

			std::unique_ptr<SceneNode> sceneRootTmp(new SceneNode(this, "root"));
			sceneRoot.swap(sceneRootTmp);

			std::unique_ptr<ViewPort> currentViewportTmp(new ViewPort(_renderTargetWidth, _renderTargetHeight, _sceneScale));
			currentViewport.swap(currentViewportTmp);

			std::unique_ptr<Camera> currentCameraTmp(new Camera("camera", currentViewport.get()));
			currentCamera.swap(currentCameraTmp);

			std::unique_ptr<ObjectFactory> objectFactoryTmp;
			objectFactory.swap(objectFactoryTmp);


			currentViewport->setScreenSize(_renderTargetWidth, _renderTargetHeight);
			currentViewport->setScale(_sceneScale);

			sceneRoot->appendObject(currentCamera.get());

			renderQueueTmp.release();
			sceneRootTmp.release();
			currentViewportTmp.release();
			currentCameraTmp.release();
			objectFactoryTmp.release();
		};


		SceneNode *SceneManager::getNodeByName(const std::string &_name)
		{
			assert(_name.size() && "node name string cannot be empty");

			auto it = namedNodes.find(_name);
			if (it != namedNodes.end())
				return (*it).second;
			return nullptr;
		};


		SceneNode *SceneManager::getRootNode()
		{
			return sceneRoot.get();
		};


		SceneNode *SceneManager::createNode(const std::string &_nodeName)
		{
			SceneNode *newNode = new SceneNode(this, _nodeName);
			assert(newNode && "scene node not created!");
			addNode(newNode);
			return newNode;
		};


		SceneNode *SceneManager::createNode(ScriptNodePtr _nodeValues)
		{
			ScriptLoader &scriptLoader = ScriptLoader::getSingleton();

			SceneNode *newNode = new SceneNode(this, scriptLoader.parseNodeName(_nodeValues));
			addNode(newNode);

			newNode->setScale(scriptLoader.parseNodeScale(_nodeValues));
			newNode->setRotation(scriptLoader.parseNodeRotation(_nodeValues));
			newNode->setPosition(scriptLoader.parseNodePosition(_nodeValues));

			return newNode;
		};


		void SceneManager::destroyNode(SceneNode *_node)
		{
			removeNode(_node);

			if (_node->getParent())
				_node->getParent()->removeChild(_node);

			_node->destroyAllChildren();
			_node->destroyAllObjects();

			delete _node;
		};


		void SceneManager::destroyObject(MovableObject *_object)
		{
			removeObject(_object);

			if (_object->getParent())
			{
				_object->getParent()->removeObject(_object);
			}

			delete _object;
		};


		Camera *SceneManager::createCamera(const std::string &_name, ScriptNodePtr _nodeValues)
		{
			throw std::logic_error("unimplemented");
		};


		MovableObject *SceneManager::createObject(ScriptNodePtr _nodeValues)
		{
			MovableObjectUPtr object = objectFactory->createObject(_nodeValues);
			addObject(object.get());
			return object.release();
		};


		SingleSprite *SceneManager::createSingleSprite(const std::string &_name, ScriptNodePtr _nodeValues)
		{
			SingleSpriteUPtr object = objectFactory->createSingleSprite(_name, _nodeValues);
			addObject(object.get());
			return object.release();
		};

		AnimatedSprite *SceneManager::createAnimatedSprite(const std::string &_name, ScriptNodePtr _nodeValues)
		{
			AnimatedSpriteUPtr object = objectFactory->createAnimatedSprite(_name, _nodeValues);
			addObject(object.get());
			return object.release();
		};

		ParticleSystem *SceneManager::createParticleSystem(const std::string &_name, ScriptNodePtr _nodeValues)
		{
			ParticleSystemUPtr object = objectFactory->createParticleSystem(_name, _nodeValues);
			addObject(object.get());
			return object.release();
		};


		SpritedText *SceneManager::createSpritedText(const std::string &_name, ScriptNodePtr _nodeValues)
		{
			SpritedTextUPtr object = objectFactory->createSpritedText(_name, _nodeValues);
			addObject(object.get());
			return object.release();
		};


		MovableObject *SceneManager::getObjectByName(const std::string &_name) const
		{
			auto it = namedObjects.find(_name);
			return it != namedObjects.end() ? (*it).second : nullptr;
		};


		Camera *SceneManager::getCurrentCamera() const
		{
			return currentCamera.get();
		};


		void SceneManager::renderScene()
		{
			if (timeValue)
				paramsManager.setTimeElapsed(timeValue->getElapsedTime());

			findVisibleRenderables();

			paramsManager.setCamera(currentCamera.get());
			paramsManager.setViewPort(currentViewport.get());

			// prepare camera and other scene related vars

			//renderSystem->getStateCashe().setVertexBuffer();
			//GraphicBuffer& vertexBuffer = renderSystem->getBatchingVertexBuffer().getId();
			//GraphicBuffer& indexBuffer = renderSystem->getIndexVertexBuffer();


			const RenderQueue::RenderableListByMaterial *rendMList = nullptr;
			MaterialManager &materialManager = MaterialManager::getSingleton();

			while ((rendMList = renderQueue->getNextPriorityRenderableList()))
			{
				for (auto it = rendMList->begin(); it != rendMList->end(); ++it)
				{
					if (!(*it).second.size())
						continue;

					// get material from first renderable, all in this list use the same
					MaterialPtr material = (*it).second.front()->getMaterial();
					const ShadingProgramParams *params = material->program->getParams();


					// setup gpu state cashe with values for this queued list
					//renderSystem->getStateCashe()->setShadingProgram(material->program->getId());
					//VertexAttribList &attribs = material->program->getAttribList();
					//renderSystem->getStateCashe()->setVertexAtribCount(material->program->getAttribList().size());


					for (int t = 0, tEnd = material->textures.size(); t < tEnd && material->textures[t]; ++t)
						renderSystem->getStateCashe()->setActiveTexture(t, material->textures[t]->getId());


					const RenderQueue::RenderableList &renderableList = (*it).second;
					bool bufferedRender(false);

					for (unsigned int i = 0; i < renderableList.size(); ++i)
					{
						paramsManager.setCurrentRenderable(renderableList[i]);
						renderSystem->render(renderableList[i], material->program.get(), &paramsManager);
					}

					// flush anything bufferable after completing material loop
					paramsManager.setCurrentMaterial(material.get());
					renderSystem->flushRenderOp(material->program.get(), &paramsManager);
				}
			}

			renderQueue->clear();
		};


		void SceneManager::setTimeControllerValue(SharedFrameTimeControllerValuePtr _timeValue)
		{
			timeValue = _timeValue;
		};



	}
}