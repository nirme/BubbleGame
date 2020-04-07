#include "AnimatorInstances.h"

namespace core
{

	AnimationControllerValue::AnimationControllerValue(Animator *_animator, float _timeSpeed) :
		FrameTimeControllerValue(_timeSpeed),
		animator(_animator)
	{};


	void AnimationControllerValue::setAnimator(std::unique_ptr<Animator> &_animator)
	{
		animator.swap(_animator);
	};


	Animator *AnimationControllerValue::getAnimator() const
	{
		return animator.get();
	};


	AnimationControllerValue::~AnimationControllerValue()
	{};


	float AnimationControllerValue::get() const
	{
		return animator->getValue();
	};


	void AnimationControllerValue::set(float _value)
	{
		FrameTimeControllerValue::set(_value);
		animator->progress(elapsedTime);
	};


	void AnimationControllerValue::resetTime()
	{
		elapsedTime = 0.0f;
	};



	float AnimatorLinear::_progressImpl(float _t)
	{
		return _t;
	};


	AnimatorLinear::AnimatorLinear(ANIMATION_MODE _mode, float _upperBound) :
			Animator(_mode, _upperBound)
	{};



	float AnimatorEaseInCircular::_progressImpl(float _t)
	{
		return 1.0f - std::sqrt(1.0f - _t * _t);
	};


	AnimatorEaseInCircular::AnimatorEaseInCircular(ANIMATION_MODE _mode, float _upperBound) :
			Animator(_mode, _upperBound)
	{};



	float AnimatorEaseOutCircular::_progressImpl(float _t)
	{
		return std::sqrt(_t * (2.0f - _t));
	};


	AnimatorEaseOutCircular::AnimatorEaseOutCircular(ANIMATION_MODE _mode, float _upperBound) :
			Animator(_mode, _upperBound)
	{};



	float AnimatorEaseInOutSine::_progressImpl(float _t)
	{
		return 0.5f - 0.5f * std::cos(_t * M_PI);
	};


	AnimatorEaseInOutSine::AnimatorEaseInOutSine(ANIMATION_MODE _mode, float _upperBound) :
			Animator(_mode, _upperBound)
	{};



	float AnimatorEaseOutInSine::_progressImpl(float _t)
	{
		return 1.0f - ( std::acos(2.0f * _t - 1.0f) / M_PI );
	};

	AnimatorEaseOutInSine::AnimatorEaseOutInSine(ANIMATION_MODE _mode, float _upperBound) :
		Animator(_mode, _upperBound)
	{};



	float AnimatorQuadraticBezier::_progressImpl(float _t)
	{
		return 0.5f - 0.5f * std::cos(_t * M_PI);
	};


	AnimatorQuadraticBezier::AnimatorQuadraticBezier( float _p1x, float _p1y, ANIMATION_MODE _mode, float _upperBound) :
			Animator(_mode, _upperBound),
	{
		setPoint(_p1x, _p1y);
	};


	void AnimatorQuadraticBezier::setPoint(float _p1x, float _p1y)
	{
		p1x = std::clamp(_p1x, 0.0f, 1.0f);
		p1y = _p1y;
	};

}