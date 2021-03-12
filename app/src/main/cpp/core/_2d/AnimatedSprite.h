#pragma once

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <cstring>


#include "../Animator.h"
#include "../AnimatorInstances.h"
#include "../SpriteCoords.h"
#include "AxisAlignedBox.h"

#include "Renderable.h"
#include "MovableObject.h"
#include "../GraphicBuffer.h"

#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../SpriteCoords.h"

#include "../ControllerManager.h"


namespace core
{
	namespace _2d
	{
		class AnimatedSprite : public MovableObject, public Renderable
		{
		public:

			class Listener
			{
			public:
				virtual void onAnimationBegin() = 0;
				virtual void onAnimationEnd() = 0;
				virtual ~Listener(){};
			};

		protected:

			typedef std::map<float,ImageSpritePtr> SpriteAnimation; // last element is null with timestamp signing length of animation
			struct SpriteAnimationStruct
			{
				SpriteAnimation animation;
				Listener *listener;
			};


			typedef std::unordered_map<std::string,SpriteAnimationStruct> AnimationSet;
			AnimationSet animationSet;


			std::string defaultAnimation;
			Animator::ANIMATION_MODE defaultMode;

			AnimationSet::iterator currentAnimation;
			float currentAnimationTime;
			Animator::ANIMATION_MODE animationMode;
			bool stopAfterAnimation;

			ImageSpritePtr currentSprite;

			ControllerPtr animationController;


			inline void updateSprite(ImageSpritePtr &sprite)
			{
				if (currentSprite != sprite)
				{
					currentSprite = sprite;
					invalidateBoundingBox();
				}
			};


			virtual AxisAlignedBox _boundingBoxImpl() const;
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;

			void playAnimation(const std::string &_name, Animator::ANIMATION_MODE _mode, bool _stopAfter, float _startTime);


		public:
			void progress(float _value);
			void setDefaultAnimation(const std::string &_defaultAnimation, Animator::ANIMATION_MODE _defaultMode);

			void addAnimation(const std::string &_animationName, const std::vector<std::pair<float, ImageSpritePtr>> &_keyframes, float _timeLength);
			void addAnimation(const std::string &_animationName, const std::map<float, ImageSpritePtr> &_keyframes, float _timeLength);
			void registerAnimationListener(const std::string &_animationName, Listener *_listener);
			void playAnimation(const std::string &_name, Animator::ANIMATION_MODE _mode, bool _stopAfter = false);
			bool pauseAnimation(bool _pause = true);
			void stopAnimation();
			bool isPaused() const;

			AnimatedSprite(const std::string &_name, Priority _renderPriority = 0, MaterialPtr _material = nullptr);
			~AnimatedSprite(){};
			MovableObject* clone(const std::string &_name) const;

			void setMaterial(ShadingProgramPtr _program, TexturePtr _spriteTexture);

			const Matrix3& getTransform() const;
			BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;

		};


		typedef std::shared_ptr<AnimatedSprite> AnimatedSpritePtr;
		typedef std::unique_ptr<AnimatedSprite> AnimatedSpriteUPtr;
	}
}
