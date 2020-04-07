#pragma once

#include <algorithm>
#include <cmath>
#include <memory>

#include "Animator.h"
#include "ControllerValues.h"


namespace core
{
	class AnimationControllerValue : public FrameTimeControllerValue
	{
	protected:
		std::unique_ptr<Animator> animator;

	public:
		AnimationControllerValue(Animator *_animator, float _timeSpeed = 1.0f);
		void setAnimator(std::unique_ptr<Animator> &_animator);
		Animator *getAnimator() const;
		virtual ~AnimationControllerValue();
		virtual float get() const;
		virtual void set(float _value);
		void resetTime();
	};

	typedef std::shared_ptr<AnimationControllerValue> AnimationControllerValuePtr;




	class AnimatorLinear : public Animator
	{
	protected:
		float _progressImpl(float _t);
	public:
		AnimatorLinear(ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
	};

	class AnimatorEaseInCircular : public Animator
	{
	protected:
		float _progressImpl(float _t);
	public:
		AnimatorEaseInCircular(ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
	};

	class AnimatorEaseOutCircular : public Animator
	{
	protected:
		float _progressImpl(float _t);
	public:
		AnimatorEaseOutCircular(ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
	};


	class AnimatorEaseInOutSine : public Animator
	{
	protected:
		float _progressImpl(float _t);
	public:
		AnimatorEaseInOutSine(ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
	};


	class AnimatorEaseOutInSine : public Animator
	{
	protected:
		float _progressImpl(float _t);
	public:
		AnimatorEaseOutInSine(ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
	};


	class AnimatorQuadraticBezier : public Animator
	{
	protected:
		float p1x, p1y;

		float _progressImpl(float _t);

	public:
		AnimatorQuadraticBezier( float _p1x = 0.5f, float _p1y = 0.5f, ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
		void setPoint(float _p1x, float _p1y);
	};
}