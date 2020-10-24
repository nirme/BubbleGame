#include "AnimatedSprite.h"


namespace core
{
	namespace _2d
	{
		AxisAlignedBox AnimatedSprite::_boundingBoxImpl() const
		{
			SpriteCoords transformedCoords;
			transformedCoords *= Vector2(currentSprite->getCoords().whRatio, 1.0f);

			transformedCoords.transform(MovableObject::getWorldTransform());
			return transformedCoords.getBoundingAABB();
		};


		void AnimatedSprite::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
		{
			_queue->addRenderable(this);
		};


		void AnimatedSprite::playAnimation(const std::string &_name, Animator::ANIMATION_MODE _mode, bool _stopAfter, float _startTime)
		{
			auto itAnimation = animationSet.find(_name);
			assert(itAnimation != animationSet.end() && "animation doesn't exist");

			if (!animationController)
			{
				SharedControllerValuePtr controllerValue = std::make_shared<TTimeframeControllerValue<AnimatedSprite>>(this);
				animationController = ControllerManager::getSingleton().createFrameTimeController(controllerValue);
			}
			else
				animationController->setEnabled(true);


			currentAnimation = itAnimation;
			animationMode = _mode;
			stopAfterAnimation = _stopAfter;

			currentAnimationTime = 0.0f;

			if ((*currentAnimation).second.listener)
				(*currentAnimation).second.listener->onAnimationBegin();

			progress(_startTime);
		};


		void AnimatedSprite::progress(float _value)
		{
			if (currentAnimation == animationSet.end())
			{
				playAnimation(defaultAnimation, defaultMode, false, 0.0f);
				return;
			}

			currentAnimationTime += _value;
			SpriteAnimation &animation = (*currentAnimation).second.animation;

			if (animationMode == Animator::AM_ONCE)
			{
				auto it = animation.upper_bound(currentAnimationTime);

				if (it != animation.end())
				{
					--it;
					updateSprite((*it).second);
				}
				else
				{
					if (stopAfterAnimation)
					{
						auto it = ++(animation.rbegin());
						updateSprite((*it).second);

						ControllerManager::getSingleton().removeController(animationController);
						animationController.reset();
					}
					else
					{
						playAnimation(defaultAnimation, defaultMode, false, currentAnimationTime - (*animation.rbegin()).first);
					}

					if ((*currentAnimation).second.listener)
						(*currentAnimation).second.listener->onAnimationBegin();
				}
			}
			else if (animationMode == Animator::AM_REPEAT)
			{
				float time = std::fmod(currentAnimationTime, (*animation.rbegin()).first);
				auto it = animation.upper_bound(time);

				--it;
				updateSprite((*it).second);
			}
			else if (animationMode == Animator::AM_LOOP)
			{
				float animationLength = (*animation.rbegin()).first;
				float time = std::fmod(currentAnimationTime, animationLength * 2.0f);

				if (time > animationLength)
					time = (animationLength * 2.0f) - time;

				auto itAnimation = animation.upper_bound(time);
				updateSprite(itAnimation != animation.end() ? (*(--itAnimation)).second : (*(++(animation.rbegin()))).second);
			}
		};


		void AnimatedSprite::setDefaultAnimation(const std::string &_defaultAnimation, Animator::ANIMATION_MODE _defaultMode)
		{
			defaultAnimation = _defaultAnimation;
			defaultMode = _defaultMode;
		};


		void AnimatedSprite::addAnimation(const std::string &_animationName, const std::vector<std::pair<float, ImageSpritePtr>> &_keyframes, float _timeLength)
		{
			assert(material && "material must be set first");

			SpriteAnimation &animation = animationSet[_animationName].animation;

			for (unsigned int i = 0, iEnd = _keyframes.size(); i < iEnd; ++i)
			{
				assert(_keyframes[i].second->getTexture() == material->textures[0] && "animation texture not matching with material");
				assert(_keyframes[i].first < _timeLength && "keyframes cannot got above animation length");

				animation.emplace(_keyframes[i].first, _keyframes[i].second);
			}

			animation.emplace(_timeLength, nullptr);
		};


		void AnimatedSprite::registerAnimationListener(const std::string &_animationName, Listener *_listener)
		{
			auto itAnimation = animationSet.find(_animationName);
			assert(itAnimation != animationSet.end() && "animation not found");
			(*itAnimation).second.listener = _listener;
		};


		void AnimatedSprite::playAnimation(const std::string &_name, Animator::ANIMATION_MODE _mode, bool _stopAfter)
		{
			playAnimation(_name, _mode, _stopAfter, 0.0f);
		};


		bool AnimatedSprite::pauseAnimation(bool _pause)
		{
			assert(animationController && currentAnimation != animationSet.end() && "no animation is playing/paused");

			if (animationController->isEnabled() == _pause)
			{
				animationController->setEnabled(!_pause);
				return true;
			}
			return false;
		};


		void AnimatedSprite::stopAnimation()
		{
			if (animationController)
				animationController->setEnabled(false);

			currentAnimation = animationSet.end();
			animationMode = defaultMode;
			stopAfterAnimation = false;
			currentAnimationTime = 0.0f;
		};


		bool AnimatedSprite::isPaused() const
		{
			return currentAnimation != animationSet.end() &&
				   animationController &&
				   !animationController->isEnabled();
		};


		AnimatedSprite::AnimatedSprite(const std::string &_name, Priority _renderPriority, MaterialPtr _material) :
				MovableObject(_name),
				Renderable(_renderPriority, _material, true)
		{};


		const Matrix3& AnimatedSprite::getTransform() const
		{
			return MovableObject::getWorldTransform();
		};


		BuffWriteResult AnimatedSprite::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			assert(!_fromSprite && "_fromSprite must be 0 for SingleSprite");

			const Matrix3 &mx = MovableObject::getWorldTransform();
			const TextureSpriteCoords &texCoords = currentSprite->getCoords();

			SpriteCoords spriteCoords(texCoords.whRatio, 1.0f);

			// 4 verts (x,y),(u,v)
			Vector2 vertices[8] = {
					mx * spriteCoords.uvPoints[0],
					texCoords.uvPoints[0],
					mx * spriteCoords.uvPoints[1],
					texCoords.uvPoints[1],
					mx * spriteCoords.uvPoints[2],
					texCoords.uvPoints[2],
					mx * spriteCoords.uvPoints[3],
					texCoords.uvPoints[3]
			};

			if (_buffer.write<>((float*)vertices, 16))
				return BuffWriteResult({ 1, true });
			return BuffWriteResult({ 0, false });
		};


	}
}
