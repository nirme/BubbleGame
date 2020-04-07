#pragma once

#include <memory>
#include "core/ControllerManager.h"
#include "core/Animator.h"
#include "core/AnimatorInstances.h"
#include "core/SoundSystem.h"
#include "core/SoundPlayer.h"


class MusicController
{
protected:
	static constexpr float MusicFadeTime = 3.0f;

	class MusicFadeAnimatorListener : public core::Animator::Listener
	{
	protected:
		MusicController *owner;

	public:
		core::SoundPlayer *fadeOutPlayer;
		core::SoundPlayer *fadeInPlayer;

		float maxVolume;

	public:
		MusicFadeAnimatorListener(MusicController *_owner, float _maxVolume = 1.0f) :
			core::Animator::Listener(),
			owner(_owner),
			fadeOutPlayer(nullptr),
			fadeInPlayer(nullptr),
			maxVolume(_maxVolume)
		{};

		virtual ~MusicFadeAnimator()
		{};

		virtual void update(float _value)
		{
			if (_value < 1.0f)
			{
				if (fadeOutPlayer)
					fadeOutPlayer->setVolume(maxVolume * _value);

				if (fadeInPlayer)
					fadeOutPlayer->setVolume(maxVolume * (1.0f - _value));
			}
			else
			{
				if (fadeOutPlayer)
				{
					fadeOutPlayer->stop();
					fadeOutPlayer = nullptr; // player is freed on stop
				}

				if (fadeInPlayer)
					fadeOutPlayer->setVolume(maxVolume);

				owner->animationComplete();
			}
		};
	};


	bool initialized;

	MusicFadeAnimatorListener fadeAnimatorListener;
	core::AnimationControllerValuePtr fadeAnimatorControllerValue;
	//std::unique_ptr<core::Animator> fadeAnimatorPtr;
	core::ControllerPtr fadeController;


	void fadeMusic(core::SoundPlayer *_fadeOutPlayer, core::SoundPlayer *_fadeInPlayer)
	{
//		fadeAnimatorPtr->setUpperBound(4.0f); //4 seconds fade effect
//		fadeAnimatorPtr->registerListener(&fadeAnimatorListener);

		fadeAnimatorControllerValue->getAnimator()->reset();
		fadeAnimatorControllerValue->resetTime();
		fadeAnimatorListener.fadeOutPlayer = _fadeOutPlayer;
		fadeAnimatorListener.fadeInPlayer = _fadeInPlayer;

		fadeController = core::ControllerManager::getSingleton().createFrameTimeController(std::static_pointer_cast<core::ControllerValue>(fadeAnimatorControllerValue));
	};


protected:
	void animationComplete()
	{
		core::ControllerManager::getSingleton().removeController(fadeController);
	};


	void initialize()
	{
		fadeAnimatorListener.maxVolume = 1.0f;
		std::unique_ptr<core::Animator> animator(new core::AnimatorEaseInOutSine(core::Animator::AM_ONCE, MusicFadeTime));

		fadeAnimatorControllerValue->setAnimator(animator);


		std::unique_ptr<core::Animator> fadeAnimatorPtr;
		core::ControllerPtr fadeController;

	};


};